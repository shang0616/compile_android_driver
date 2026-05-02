// SPDX-License-Identifier: GPL-2.0
/*
 * TearGame 内存操作模块 v2.0
 * 
 * 基于物理页表的内存读写，包含：
 * - 大页支持 (2MB/1GB)
 * - VMA安全检查（防止读取陷阱地址）
 * - 多层缺页检测
 * - 物理地址验证
 * - 增强页表遍历缓存（LRU策略）
 * - ARM64 NEON加速内存拷贝
 * - 智能预取策略
 */

#include "teargame.h"
#include "teargame_security.h"
#include <linux/highmem.h>
#include <linux/io.h>
#include <linux/mm.h>
#include <linux/sched/mm.h>
#include <linux/sched/task.h>
#include <linux/percpu.h>
#include <linux/prefetch.h>
#include <linux/slab.h>
#include <asm/pgtable.h>
#include <asm/io.h>

#ifdef CONFIG_ARM64
#include <asm/neon.h>
#endif

/*
 * ============================================================================
 * 私有常量
 * ============================================================================
 */

/* 每次页表遍历的最大块大小 */
#define TEAR_CHUNK_SIZE         PAGE_SIZE

/* 内存映射标志 */
#define TEAR_REMAP_FLAGS        MEMREMAP_WB

/* 增强缓存配置 */
#define TEAR_PTW_CACHE_SIZE_V2  16
#define TEAR_PTW_CACHE_EXPIRY_V2 (HZ / 5)   /* 200ms过期 */

/* 智能预取配置 */
#define TEAR_PREFETCH_CONFIDENCE_THRESHOLD  3
#define TEAR_PREFETCH_MAX_STRIDE           (PAGE_SIZE * 4)

/*
 * ============================================================================
 * NEON 加速内存拷贝 (ARM64)
 * ============================================================================
 */

#ifdef CONFIG_ARM64

/*
 * NEON 加速的内存拷贝
 * 使用 NEON 寄存器一次拷贝 64 字节
 */
static __maybe_unused void tear_memcpy_neon(void *dst, const void *src, size_t n)
{
    /* 只在非中断上下文且数据足够大时使用 NEON */
    if (n >= 64 && !in_interrupt() && !irqs_disabled()) {
        kernel_neon_begin();
        
        /* 使用 NEON 寄存器批量拷贝 */
        while (n >= 64) {
            asm volatile(
                "ldp q0, q1, [%1]\n"
                "ldp q2, q3, [%1, #32]\n"
                "stp q0, q1, [%0]\n"
                "stp q2, q3, [%0, #32]\n"
                : 
                : "r"(dst), "r"(src)
                : "memory", "v0", "v1", "v2", "v3"
            );
            dst += 64;
            src += 64;
            n -= 64;
        }
        
        kernel_neon_end();
    }
    
    /* 剩余部分使用普通拷贝 */
    if (n > 0)
        memcpy(dst, src, n);
}

#define tear_fast_memcpy tear_memcpy_neon

#else /* !CONFIG_ARM64 */

#define tear_fast_memcpy memcpy

#endif /* CONFIG_ARM64 */

/*
 * ============================================================================
 * 增强页表缓存 (Page Table Walk Cache v2.0)
 * ============================================================================
 * 特性:
 * - 更大的缓存容量 (64条/CPU)
 * - LRU 替换策略
 * - 命中计数统计
 * - 更长的过期时间
 */

#if TEAR_ENABLE_PTW_CACHE

/* 增强缓存条目结构 */
struct tear_ptw_cache_entry_v2 {
    pid_t           pid;            /* 进程ID */
    unsigned long   vaddr_page;     /* 虚拟地址（页对齐） */
    phys_addr_t     phys_page;      /* 物理地址（页对齐） */
    unsigned long   page_size;      /* 页大小 */
    unsigned long   timestamp;      /* 缓存时间 (jiffies) */
    unsigned int    hit_count;      /* 命中计数（用于LRU） */
    bool            valid;          /* 是否有效 */
};

/* Per-CPU 增强缓存结构 */
struct tear_ptw_cache_v2 {
    struct tear_ptw_cache_entry_v2 entries[TEAR_PTW_CACHE_SIZE_V2];
    unsigned int    next_slot;      /* 下一个写入槽位 */
    unsigned long   total_hits;     /* 总命中数 */
    unsigned long   total_misses;   /* 总未命中数 */
};

/* 定义 per-CPU 缓存变量 */
static struct tear_ptw_cache_v2 __percpu *ptw_cache_v2;

/*
 * LRU 槽位选择
 * 找到最少使用的槽位
 */
static unsigned int ptw_cache_find_lru_slot(struct tear_ptw_cache_v2 *cache)
{
    unsigned int min_hits = UINT_MAX;
    unsigned int lru_slot = 0;
    unsigned long now = jiffies;
    int i;
    
    for (i = 0; i < TEAR_PTW_CACHE_SIZE_V2; i++) {
        struct tear_ptw_cache_entry_v2 *e = &cache->entries[i];
        
        /* 优先选择无效槽位 */
        if (!e->valid)
            return i;
        
        /* 选择过期的槽位 */
        if (time_after(now, e->timestamp + TEAR_PTW_CACHE_EXPIRY_V2))
            return i;
        
        /* 选择命中最少的槽位 */
        if (e->hit_count < min_hits) {
            min_hits = e->hit_count;
            lru_slot = i;
        }
    }
    
    return lru_slot;
}

/*
 * 查找缓存（增强版）
 */
static bool ptw_cache_lookup(pid_t pid, unsigned long vaddr,
                             phys_addr_t *phys_out, unsigned long *page_size_out)
{
    struct tear_ptw_cache_v2 *cache;
    unsigned long vaddr_page = vaddr & PAGE_MASK;
    unsigned long now = jiffies;
    int i;
    
    preempt_disable();
    cache = this_cpu_ptr(ptw_cache_v2);
    
    for (i = 0; i < TEAR_PTW_CACHE_SIZE_V2; i++) {
        struct tear_ptw_cache_entry_v2 *e = &cache->entries[i];
        
        if (!e->valid)
            continue;
        
        /* 检查是否过期 */
        if (time_after(now, e->timestamp + TEAR_PTW_CACHE_EXPIRY_V2)) {
            e->valid = false;
            continue;
        }
        
        /* 检查匹配 */
        if (e->pid == pid && e->vaddr_page == vaddr_page) {
            unsigned long offset = vaddr & (e->page_size - 1);
            *phys_out = e->phys_page + offset;
            if (page_size_out)
                *page_size_out = e->page_size;
            
            /* 更新命中计数 */
            e->hit_count++;
            cache->total_hits++;
            
            preempt_enable();
            return true;
        }
    }
    
    cache->total_misses++;
    preempt_enable();
    return false;
}

/*
 * 更新缓存（LRU策略）
 */
static void ptw_cache_update(pid_t pid, unsigned long vaddr,
                             phys_addr_t phys, unsigned long page_size)
{
    struct tear_ptw_cache_v2 *cache;
    struct tear_ptw_cache_entry_v2 *e;
    unsigned int slot;
    
    preempt_disable();
    cache = this_cpu_ptr(ptw_cache_v2);
    
    /* 使用LRU策略选择槽位 */
    slot = ptw_cache_find_lru_slot(cache);
    
    e = &cache->entries[slot];
    e->pid = pid;
    e->vaddr_page = vaddr & PAGE_MASK;
    e->phys_page = phys & PAGE_MASK;
    e->page_size = page_size;
    e->timestamp = jiffies;
    e->hit_count = 1;
    e->valid = true;
    
    preempt_enable();
}

/*
 * 使指定进程的缓存失效
 */
static void ptw_cache_invalidate_pid(pid_t pid)
{
    int cpu;
    
    for_each_possible_cpu(cpu) {
        struct tear_ptw_cache_v2 *cache = per_cpu_ptr(ptw_cache_v2, cpu);
        int i;
        
        for (i = 0; i < TEAR_PTW_CACHE_SIZE_V2; i++) {
            if (cache->entries[i].pid == pid)
                cache->entries[i].valid = false;
        }
    }
}

/*
 * 清空所有缓存
 */
static void ptw_cache_flush_all(void)
{
    int cpu;
    
    for_each_possible_cpu(cpu) {
        struct tear_ptw_cache_v2 *cache = per_cpu_ptr(ptw_cache_v2, cpu);
        memset(cache, 0, sizeof(*cache));
    }
}

/*
 * 获取缓存统计信息
 */
void tear_ptw_cache_stats(unsigned long *hits, unsigned long *misses)
{
    int cpu;
    unsigned long total_hits = 0, total_misses = 0;
    
    for_each_possible_cpu(cpu) {
        struct tear_ptw_cache_v2 *cache = per_cpu_ptr(ptw_cache_v2, cpu);
        total_hits += cache->total_hits;
        total_misses += cache->total_misses;
    }
    
    if (hits)
        *hits = total_hits;
    if (misses)
        *misses = total_misses;
}

#endif /* TEAR_ENABLE_PTW_CACHE */

/*
 * ============================================================================
 * 智能预取系统
 * ============================================================================
 * 根据访问模式自动预取后续页面
 */

#if TEAR_ENABLE_PREFETCH

/* 预取状态结构 */
struct tear_prefetch_state {
    unsigned long last_addr;    /* 上次访问地址 */
    long stride;                /* 检测到的步长 */
    int confidence;             /* 置信度 */
    pid_t pid;                  /* 关联进程 */
};

/* Per-CPU 预取状态 */
static struct tear_prefetch_state __percpu *prefetch_state;

/*
 * 智能预取
 * 检测访问模式并预取后续页面
 */
static __maybe_unused void smart_prefetch(pid_t pid, struct mm_struct *mm, unsigned long addr)
{
    struct tear_prefetch_state *ps;
    long detected_stride;
    int i;
    
    preempt_disable();
    ps = this_cpu_ptr(prefetch_state);
    
    /* 如果切换了进程，重置状态 */
    if (ps->pid != pid) {
        ps->pid = pid;
        ps->last_addr = addr;
        ps->stride = 0;
        ps->confidence = 0;
        preempt_enable();
        return;
    }
    
    /* 计算步长 */
    detected_stride = (long)(addr - ps->last_addr);
    
    /* 检测访问模式 */
    if (detected_stride == ps->stride && 
        detected_stride != 0 &&
        detected_stride > -(long)TEAR_PREFETCH_MAX_STRIDE &&
        detected_stride < (long)TEAR_PREFETCH_MAX_STRIDE) {
        ps->confidence++;
    } else {
        ps->stride = detected_stride;
        ps->confidence = 1;
    }
    
    ps->last_addr = addr;
    
    /* 置信度足够时执行预取 */
    if (ps->confidence >= TEAR_PREFETCH_CONFIDENCE_THRESHOLD && 
        ps->stride != 0) {
        phys_addr_t prefetch_phys;
        
        for (i = 1; i <= TEAR_PREFETCH_STRIDE; i++) {
            unsigned long pf_addr = addr + ps->stride * i;
            
            /* 预先进行地址转换以填充缓存 */
            if (!ptw_cache_lookup(pid, pf_addr, &prefetch_phys, NULL)) {
                prefetch_phys = tear_vaddr_to_phys_secure(mm, pf_addr, NULL, false);
                if (prefetch_phys)
                    ptw_cache_update(pid, pf_addr, prefetch_phys, PAGE_SIZE);
            }
        }
    }
    
    preempt_enable();
}

#endif /* TEAR_ENABLE_PREFETCH */

/*
 * ============================================================================
 * 页表遍历 - ARM64优化版
 * ============================================================================
 */

/*
 * 将虚拟地址转换为物理地址（带页大小检测）
 * 支持4KB普通页、2MB大页、1GB大页
 */
phys_addr_t tear_vaddr_to_phys(struct mm_struct *mm, unsigned long vaddr,
                               unsigned long *page_size)
{
    pgd_t *pgd;
    p4d_t *p4d;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *pte;
    phys_addr_t phys = 0;
    
    if (!mm || !mm->pgd)
        return 0;
    
    /* 默认4KB页 */
    if (page_size)
        *page_size = PAGE_SIZE;
    
    /* 第0级: PGD */
    pgd = pgd_offset(mm, vaddr);
    if (pgd_none(*pgd) || pgd_bad(*pgd))
        return 0;
    
    /* 第1级: P4D (ARM64通常折叠) */
    p4d = p4d_offset(pgd, vaddr);
    if (p4d_none(*p4d) || p4d_bad(*p4d))
        return 0;
    
    /* 第2级: PUD */
    pud = pud_offset(p4d, vaddr);
    if (pud_none(*pud))
        return 0;
    
    /* 检查1GB大页 */
    if (tear_pud_huge(*pud)) {
        if (page_size)
            *page_size = PUD_SIZE;
        phys = (pud_pfn(*pud) << PAGE_SHIFT) | (vaddr & ~PUD_MASK);
        return phys;
    }
    
    if (pud_bad(*pud))
        return 0;
    
    /* 第3级: PMD */
    pmd = pmd_offset(pud, vaddr);
    if (pmd_none(*pmd))
        return 0;
    
    /* 检查2MB大页 */
    if (tear_pmd_huge(*pmd)) {
        if (page_size)
            *page_size = PMD_SIZE;
        phys = (pmd_pfn(*pmd) << PAGE_SHIFT) | (vaddr & ~PMD_MASK);
        return phys;
    }
    
    if (pmd_bad(*pmd))
        return 0;
    
    /* 第4级: PTE */
    pte = tear_pte_offset_map(pmd, vaddr);
    if (!pte)
        return 0;
    
    if (pte_none(*pte) || !pte_present(*pte)) {
        tear_pte_unmap(pte);
        return 0;
    }
    
    phys = (pte_pfn(*pte) << PAGE_SHIFT) | (vaddr & ~PAGE_MASK);
    tear_pte_unmap(pte);
    
    return phys;
}

/*
 * ============================================================================
 * 物理内存映射辅助函数
 * ============================================================================
 */

/*
 * 映射物理地址到内核空间
 */
static __maybe_unused void *tear_map_phys(phys_addr_t phys, size_t size, bool *use_ioremap)
{
    void *mapped;
    
    *use_ioremap = false;
    
    /* 验证PFN有效性 */
    if (!tear_pfn_valid(phys >> PAGE_SHIFT)) {
        tear_debug("物理映射: PFN无效 phys=0x%llx\n", (unsigned long long)phys);
        return NULL;
    }
    
#if TEAR_SECURITY_SKIP_RESERVED
    /* 安全检查: 跳过保留页 */
    if (!tear_is_phys_safe(phys)) {
        tear_debug("物理映射: 安全检查失败 phys=0x%llx\n", (unsigned long long)phys);
        return NULL;
    }
#endif
    
    /* 优先使用memremap */
    mapped = memremap(phys & PAGE_MASK, PAGE_SIZE, TEAR_REMAP_FLAGS);
    if (mapped)
        return mapped + (phys & ~PAGE_MASK);
    
    /* 后备: 使用ioremap */
#ifdef CONFIG_ARM64
    mapped = ioremap_cache(phys & PAGE_MASK, PAGE_SIZE);
#else
    mapped = ioremap(phys & PAGE_MASK, PAGE_SIZE);
#endif
    
    if (mapped) {
        *use_ioremap = true;
        return mapped + (phys & ~PAGE_MASK);
    }
    
    return NULL;
}

/*
 * 解除物理地址映射
 */
static __maybe_unused void tear_unmap_phys(void *mapped, bool use_ioremap)
{
    void *page_start = (void *)((unsigned long)mapped & PAGE_MASK);
    
    if (use_ioremap)
        iounmap(page_start);
    else
        memunmap(page_start);
}

/*
 * ============================================================================
 * 内存读取实现
 * ============================================================================
 */

/*
 * 带缓存的地址转换
 */
static __maybe_unused phys_addr_t tear_vaddr_to_phys_cached(pid_t pid, struct mm_struct *mm,
                                                            unsigned long addr,
                                                            unsigned long *page_size)
{
    phys_addr_t phys;
    unsigned long ps = PAGE_SIZE;
    
#if TEAR_ENABLE_PTW_CACHE
    /* 先查缓存 */
    if (ptw_cache_lookup(pid, addr, &phys, &ps)) {
        if (page_size)
            *page_size = ps;
        return phys;
    }
#endif
    
    /* 缓存未命中，执行页表遍历 */
    phys = tear_vaddr_to_phys_secure(mm, addr, &ps, false);
    
#if TEAR_ENABLE_PTW_CACHE
    /* 更新缓存 */
    if (phys != 0)
        ptw_cache_update(pid, addr, phys, ps);
#endif
    
    if (page_size)
        *page_size = ps;
    
    return phys;
}

/*
 * 从目标进程读取内存（带安全检查和性能优化）
 */
bool read_process_memory(pid_t pid, unsigned long addr,
                         void __user *buffer, size_t size)
{
    if (size > TEAR_MAX_SAFE_RW_SIZE)
        size = TEAR_MAX_SAFE_RW_SIZE;
    return read_safe(pid, addr, buffer, size);
}

/*
 * ============================================================================
 * 内存写入实现
 * ============================================================================
 */

/*
 * 向目标进程写入内存（带安全检查）
 */
bool write_process_memory(pid_t pid, unsigned long addr,
                          void __user *buffer, size_t size)
{
    if (size > TEAR_MAX_SAFE_RW_SIZE)
        size = TEAR_MAX_SAFE_RW_SIZE;
    return write_safe(pid, addr, buffer, size);
}

/*
 * ============================================================================
 * 模块初始化/清理
 * ============================================================================
 */

int teargame_memory_init(void)
{
#if TEAR_ENABLE_PTW_CACHE
    /* 动态分配 per-CPU 页表缓存 */
    ptw_cache_v2 = alloc_percpu(struct tear_ptw_cache_v2);
    if (!ptw_cache_v2) {
        tear_warn("Failed to allocate percpu ptw_cache_v2\n");
        return -ENOMEM;
    }
    ptw_cache_flush_all();
#endif

#if TEAR_ENABLE_PREFETCH
    prefetch_state = alloc_percpu(struct tear_prefetch_state);
    if (!prefetch_state) {
        tear_warn("Failed to allocate percpu prefetch_state\n");
#if TEAR_ENABLE_PTW_CACHE
        free_percpu(ptw_cache_v2);
#endif
        return -ENOMEM;
    }
#endif

    return 0;
}

void teargame_memory_exit(void)
{
#if TEAR_ENABLE_PTW_CACHE
    if (ptw_cache_v2) {
        unsigned long hits, misses;
        tear_ptw_cache_stats(&hits, &misses);
        tear_debug("页表缓存统计: 命中=%lu, 未命中=%lu, 命中率=%.1f%%\n",
                   hits, misses, 
                   (hits + misses) > 0 ? 
                   (100.0 * hits / (hits + misses)) : 0.0);
        free_percpu(ptw_cache_v2);
        ptw_cache_v2 = NULL;
    }
#endif

#if TEAR_ENABLE_PREFETCH
    if (prefetch_state) {
        free_percpu(prefetch_state);
        prefetch_state = NULL;
    }
#endif

    tear_debug("内存子系统已清理\n");
}

/* 导出函数供其他模块使用 */
void tear_memory_invalidate_pid(pid_t pid)
{
#if TEAR_ENABLE_PTW_CACHE
    ptw_cache_invalidate_pid(pid);
#endif
}
