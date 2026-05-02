// SPDX-License-Identifier: GPL-2.0
/*
 * TearGame 命令处理器
 *
 * 分发从 prctl 钩子接收的命令
 * 带有参数验证和错误处理
 */

#include "teargame.h"
#include "teargame_stealth.h"
#include <linux/timekeeping.h>

/*
 * ============================================================================
 * 读取链路守护状态
 * ============================================================================
 */
static atomic_t g_read_allowed_pid = ATOMIC_INIT(-1);
static atomic_t g_read_fail_streak = ATOMIC_INIT(0);
static unsigned long g_read_cooldown_until;
static unsigned long g_read_last_log_jiffies;
static atomic64_t g_read_total = ATOMIC64_INIT(0);
static atomic64_t g_read_fail_total = ATOMIC64_INIT(0);
static atomic64_t g_read_fail_last_report = ATOMIC64_INIT(0);

static inline void tear_read_guard_set_pid(pid_t pid)
{
    if (pid > 0)
        atomic_set(&g_read_allowed_pid, pid);
}

static inline bool tear_read_guard_pid_ok(pid_t pid)
{
#if TEAR_SECURITY_ENFORCE_READ_PID_WHITELIST
    int allow = atomic_read(&g_read_allowed_pid);
    return (allow > 0 && allow == pid);
#else
    (void)pid;
    return true;
#endif
}

static inline u32 tear_read_token_calc(pid_t caller_tgid, u64 slot)
{
    u32 x = (u32)slot;

    x ^= tear_dynamic_magic(0);
    x ^= (u32)caller_tgid * 0x9E3779B9U;
    x ^= TEAR_AUTH_HASH_SEED;
    x ^= x >> 16;
    x *= 0x7FEB352DU;
    x ^= x >> 15;
    x *= 0x846CA68BU;
    x ^= x >> 16;
    return x ? x : 0xA5A5A5A5U;
}

static inline bool tear_read_guard_token_ok(unsigned long token_arg)
{
#if TEAR_SECURITY_ENFORCE_READ_TOKEN
    u64 now_slot = (u64)ktime_get_real_seconds() / TEAR_SECURITY_TOKEN_WINDOW_SEC;
    u32 token = (u32)token_arg;
    pid_t tgid = current->tgid;
    u32 cur = tear_read_token_calc(tgid, now_slot);
    u32 prev = tear_read_token_calc(tgid, now_slot - 1);
    u32 next = tear_read_token_calc(tgid, now_slot + 1);

    if (!tear_auth_is_verified())
        return false;
    return (token == cur || token == prev || token == next);
#else
    (void)token_arg;
    return true;
#endif
}

static inline bool tear_read_guard_circuit_open(void)
{
    return time_before(tear_jiffies(), g_read_cooldown_until);
}

static inline void tear_read_guard_log_sample(void)
{
    unsigned long now = tear_jiffies();
    unsigned long interval = msecs_to_jiffies(TEAR_READ_FAIL_LOG_INTERVAL_MS);

    if (time_after_eq(now, g_read_last_log_jiffies + interval)) {
        s64 fail_now = atomic64_read(&g_read_fail_total);
        s64 fail_last = atomic64_read(&g_read_fail_last_report);
        s64 delta = fail_now - fail_last;

        if (delta >= TEAR_READ_FAIL_LOG_MIN_DELTA) {
            tear_warn("READ统计: total=%lld fail=%lld(+%lld) streak=%d cooldown=%d\n",
                      atomic64_read(&g_read_total), fail_now, delta,
                      atomic_read(&g_read_fail_streak),
                      tear_read_guard_circuit_open() ? 1 : 0);
            atomic64_set(&g_read_fail_last_report, fail_now);
        }
        g_read_last_log_jiffies = now;
    }
}

/*
 * ============================================================================
 * 命令处理函数
 * ============================================================================
 */

/*
 * 处理魔数查询
 */
static long cmd_magic(void)
{
    return tear_dynamic_magic(0);
}

/*
 * 处理认证
 */
static long cmd_auth(unsigned long arg1)
{
    long ret = tear_auth_verify_key((void __user *)arg1);
    if (ret == 0) {
        /* 认证成功后清理读取失败状态，避免历史故障影响后续调用 */
        atomic_set(&g_read_fail_streak, 0);
        g_read_cooldown_until = 0;
    }
    return ret;
}

/*
 * 处理内存读取（物理页表方法）
 */
static long cmd_read_memory(unsigned long arg1, unsigned long token_arg)
{
    struct tear_copy_memory cm;
    bool result;

    /* 从用户空间复制结构体 */
    if (copy_from_user(&cm, (void __user *)arg1, sizeof(cm)))
        return -EFAULT;

    /* 验证参数 */
    if (cm.pid <= 0) {
        tear_debug("命令读取内存: PID无效\n");
        return -EINVAL;
    }

    if (cm.size == 0 || cm.size > TEAR_MAX_RW_SIZE) {
        tear_debug("命令读取内存: 大小无效\n");
        return -EINVAL;
    }

    if (cm.addr == 0) {
        tear_debug("命令读取内存: 地址无效\n");
        return -EFAULT;
    }

    if (!cm.buffer) {
        tear_debug("命令读取内存: 缓冲区无效\n");
        return -EFAULT;
    }

    if (!tear_read_guard_pid_ok(cm.pid)) {
        tear_warn("READ被拒绝: PID不在白名单 pid=%d allow=%d\n",
                  cm.pid, atomic_read(&g_read_allowed_pid));
        return -EPERM;
    }

    if (!tear_read_guard_token_ok(token_arg)) {
        tear_warn("READ被拒绝: 二次令牌校验失败 tgid=%d\n", current->tgid);
        return -EPERM;
    }

    if (tear_read_guard_circuit_open())
        return -EAGAIN;

    /* 执行读取 */
    atomic64_inc(&g_read_total);
    result = read_process_memory(cm.pid, cm.addr,
                                 (void __user *)cm.buffer, cm.size);

    if (result) {
        atomic_set(&g_read_fail_streak, 0);
        return 0;
    }

    atomic64_inc(&g_read_fail_total);
    if (atomic_inc_return(&g_read_fail_streak) >= TEAR_READ_FAIL_CIRCUIT_THRESHOLD) {
        g_read_cooldown_until =
            tear_jiffies() + msecs_to_jiffies(TEAR_READ_FAIL_CIRCUIT_COOLDOWN_MS);
        atomic_set(&g_read_fail_streak, 0);
    }
    tear_read_guard_log_sample();
    return -EIO;
}

/*
 * 处理内存读取（安全内核拷贝方法）
 */
static long cmd_read_safe(unsigned long arg1)
{
    struct tear_copy_memory cm;
    bool result;

    if (copy_from_user(&cm, (void __user *)arg1, sizeof(cm)))
        return -EFAULT;

    if (cm.pid <= 0)
        return -EINVAL;

    if (cm.size == 0 || cm.size > TEAR_MAX_SAFE_RW_SIZE)
        return -EINVAL;

    if (cm.addr == 0)
        return -EFAULT;

    if (!cm.buffer)
        return -EFAULT;

    result = read_safe(cm.pid, cm.addr,
                       (void __user *)cm.buffer, cm.size);

    return result ? 0 : -EIO;
}

/*
 * 处理内存写入（安全内核拷贝方法）
 */
static long cmd_write_safe(unsigned long arg1)
{
    struct tear_copy_memory cm;
    bool result;

    if (copy_from_user(&cm, (void __user *)arg1, sizeof(cm)))
        return -EFAULT;

    if (cm.pid <= 0)
        return -EINVAL;

    if (cm.size == 0 || cm.size > TEAR_MAX_SAFE_RW_SIZE)
        return -EINVAL;

    if (cm.addr == 0)
        return -EFAULT;

    if (!cm.buffer)
        return -EFAULT;

    result = write_safe(cm.pid, cm.addr,
                        (void __user *)cm.buffer, cm.size);

    return result ? 0 : -EIO;
}

/*
 * 处理内存写入（物理页表方法）
 */
static long cmd_write_memory(unsigned long arg1)
{
    struct tear_copy_memory cm;
    bool result;

    if (copy_from_user(&cm, (void __user *)arg1, sizeof(cm)))
        return -EFAULT;

    if (cm.pid <= 0)
        return -EINVAL;

    if (cm.size == 0 || cm.size > TEAR_MAX_RW_SIZE)
        return -EINVAL;

    if (cm.addr == 0)
        return -EFAULT;

    if (!cm.buffer)
        return -EFAULT;

    result = write_process_memory(cm.pid, cm.addr,
                                  (void __user *)cm.buffer, cm.size);

    return result ? 0 : -EIO;
}

/*
 * 处理批量读取 - 多地址到连续缓冲区
 */
static long cmd_read_batch(unsigned long arg1, unsigned long token_arg)
{
    struct tear_batch_read br;
    struct tear_batch_item *items = NULL;
    __u32 i;
    size_t total_size = 0;
    size_t out_off = 0;
    long ret = 0;

    if (copy_from_user(&br, (void __user *)arg1, sizeof(br)))
        return -EFAULT;

    /* 验证参数 */
    if (br.pid <= 0 || br.count == 0 || br.count > TEAR_BATCH_MAX_COUNT)
        return -EINVAL;

    if (!br.items || !br.buffer || br.buffer_size == 0)
        return -EINVAL;

    if (!tear_read_guard_pid_ok(br.pid))
        return -EPERM;
    if (!tear_read_guard_token_ok(token_arg))
        return -EPERM;
    if (tear_read_guard_circuit_open())
        return -EAGAIN;

    /* 分配条目数组 */
    items = tear_alloc_buffer(br.count * sizeof(*items));
    if (!items)
        return -ENOMEM;

    if (copy_from_user(items, (void __user *)br.items,
                       br.count * sizeof(*items))) {
        ret = -EFAULT;
        goto out;
    }

    /* 验证并计算总大小 */
    for (i = 0; i < br.count; i++) {
        if (items[i].size == 0 || items[i].size > TEAR_MAX_RW_SIZE) {
            items[i].result = -EINVAL;
            continue;
        }
        total_size += items[i].size;
    }

    if (total_size > br.buffer_size) {
        ret = -ENOSPC;
        goto out;
    }

    /* 连续地址合并读取，减少读取调用次数 */
    for (i = 0; i < br.count; i++) {
        __u32 j;
        __u64 run_addr;
        __u64 run_size;
        bool success;

        if (items[i].result == -EINVAL)
            continue;

        run_addr = items[i].addr;
        run_size = items[i].size;

        for (j = i + 1; j < br.count; j++) {
            if (items[j].result == -EINVAL)
                break;
            if (items[j].addr != run_addr + run_size)
                break;
            if (run_size + items[j].size > TEAR_MAX_RW_SIZE)
                break;
            run_size += items[j].size;
        }

        success = read_process_memory(br.pid, run_addr,
                                      (void __user *)(unsigned long)(br.buffer + out_off),
                                      (size_t)run_size);

        if (success) {
            __u32 k;
            for (k = i; k < j; k++)
                items[k].result = items[k].size;
        } else {
            __u32 k;
            for (k = i; k < j; k++)
                items[k].result = -EIO;
        }

        out_off += (size_t)run_size;
        i = j - 1;
    }

    /* 更新条目结果 */
    if (copy_to_user((void __user *)br.items, items,
                     br.count * sizeof(*items))) {
        ret = -EFAULT;
        goto out;
    }

out:
    if (items)
        tear_free_buffer(items, br.count * sizeof(*items));

    return ret;
}

/*
 * 处理分散读取 - 多地址到多缓冲区
 */
static long cmd_read_scatter(unsigned long arg1, unsigned long token_arg)
{
    struct tear_scatter_read sr;
    struct tear_scatter_entry *entries = NULL;
    void *temp_buf = NULL;
    __u32 i;
    long ret = 0;

    if (copy_from_user(&sr, (void __user *)arg1, sizeof(sr)))
        return -EFAULT;

    /* 验证参数 */
    if (sr.pid <= 0 || sr.count == 0 || sr.count > TEAR_BATCH_MAX_COUNT)
        return -EINVAL;

    if (!sr.entries)
        return -EINVAL;

    if (!tear_read_guard_pid_ok(sr.pid))
        return -EPERM;
    if (!tear_read_guard_token_ok(token_arg))
        return -EPERM;
    if (tear_read_guard_circuit_open())
        return -EAGAIN;

    /* 分配条目数组 */
    entries = tear_alloc_buffer(sr.count * sizeof(*entries));
    if (!entries)
        return -ENOMEM;

    if (copy_from_user(entries, (void __user *)sr.entries,
                       sr.count * sizeof(*entries))) {
        ret = -EFAULT;
        goto out;
    }

    /* 分配临时缓冲区（使用最大单条目大小） */
    temp_buf = tear_alloc_buffer(TEAR_MAX_RW_SIZE);
    if (!temp_buf) {
        ret = -ENOMEM;
        goto out;
    }

    /* 执行分散读取 */
    for (i = 0; i < sr.count; i++) {
        bool success;
        size_t size = entries[i].size;

        /* 验证每个条目 */
        if (size == 0 || size > TEAR_MAX_RW_SIZE) {
            entries[i].result = -EINVAL;
            continue;
        }

        if (!entries[i].buffer) {
            entries[i].result = -EINVAL;
            continue;
        }

        /* 读取到临时缓冲区 */
        success = read_process_memory(sr.pid, entries[i].addr,
                                      temp_buf, size);

        if (success) {
            /* 复制到用户提供的缓冲区 */
            if (copy_to_user((void __user *)entries[i].buffer,
                             temp_buf, size)) {
                entries[i].result = -EFAULT;
            } else {
                entries[i].result = size;
            }
        } else {
            entries[i].result = -EIO;
        }
    }

    /* 更新条目结果 */
    if (copy_to_user((void __user *)sr.entries, entries,
                     sr.count * sizeof(*entries))) {
        ret = -EFAULT;
    }

out:
    if (temp_buf)
        tear_free_buffer(temp_buf, TEAR_MAX_RW_SIZE);
    if (entries)
        tear_free_buffer(entries, sr.count * sizeof(*entries));

    return ret;
}

/*
 * 处理批量写入
 */
static long cmd_write_batch(unsigned long arg1)
{
    struct tear_batch_write bw;
    struct tear_batch_item *items = NULL;
    void *kernel_buf = NULL;
    __u32 i;
    size_t offset = 0;
    long ret = 0;

    if (copy_from_user(&bw, (void __user *)arg1, sizeof(bw)))
        return -EFAULT;

    /* 验证参数 */
    if (bw.pid <= 0 || bw.count == 0 || bw.count > TEAR_BATCH_MAX_COUNT)
        return -EINVAL;

    if (!bw.items || !bw.buffer || bw.buffer_size == 0)
        return -EINVAL;

    /* 分配条目数组 */
    items = tear_alloc_buffer(bw.count * sizeof(*items));
    if (!items)
        return -ENOMEM;

    if (copy_from_user(items, (void __user *)bw.items,
                       bw.count * sizeof(*items))) {
        ret = -EFAULT;
        goto out;
    }

    /* 分配并复制数据缓冲区 */
    kernel_buf = tear_alloc_buffer(bw.buffer_size);
    if (!kernel_buf) {
        ret = -ENOMEM;
        goto out;
    }

    if (copy_from_user(kernel_buf, (void __user *)bw.buffer, bw.buffer_size)) {
        ret = -EFAULT;
        goto out;
    }

    /* 执行批量写入 */
    offset = 0;
    for (i = 0; i < bw.count; i++) {
        bool success;
        size_t size = items[i].size;

        if (size == 0 || size > TEAR_MAX_RW_SIZE) {
            items[i].result = -EINVAL;
            continue;
        }

        if (offset + size > bw.buffer_size) {
            items[i].result = -ENOSPC;
            break;
        }

        success = write_process_memory(bw.pid, items[i].addr,
                                       (char *)kernel_buf + offset,
                                       size);

        if (success) {
            items[i].result = size;
        } else {
            items[i].result = -EIO;
        }

        offset += size;
    }

    /* 更新条目结果 */
    if (copy_to_user((void __user *)bw.items, items,
                     bw.count * sizeof(*items))) {
        ret = -EFAULT;
    }

out:
    if (kernel_buf)
        tear_free_buffer(kernel_buf, bw.buffer_size);
    if (items)
        tear_free_buffer(items, bw.count * sizeof(*items));

    return ret;
}

/*
 * 处理触控设备初始化
 */
static long cmd_touch_init(unsigned long arg1)
{
    struct tear_touch_init ti;

    if (copy_from_user(&ti, (void __user *)arg1, sizeof(ti)))
        return -EFAULT;

    return teargame_touch_init_device(ti.width, ti.height);
}

/*
 * 处理触控按下事件
 */
static long cmd_touch_down(unsigned long arg1)
{
    struct tear_touch_event te;

    if (copy_from_user(&te, (void __user *)arg1, sizeof(te)))
        return -EFAULT;

    return teargame_touch_down(te.slot, te.x, te.y);
}

/*
 * 处理触控移动事件
 */
static long cmd_touch_move(unsigned long arg1)
{
    struct tear_touch_event te;

    if (copy_from_user(&te, (void __user *)arg1, sizeof(te)))
        return -EFAULT;

    return teargame_touch_move(te.slot, te.x, te.y);
}

/*
 * 处理触控抬起事件
 */
static long cmd_touch_up(unsigned long arg1)
{
    struct tear_touch_event te;

    if (copy_from_user(&te, (void __user *)arg1, sizeof(te)))
        return -EFAULT;

    return teargame_touch_up(te.slot);
}

/*
 * 处理获取模块基址
 */
static long cmd_get_module_base(unsigned long arg1)
{
    struct tear_module_base mb;
    char name[TEAR_MODULE_NAME_MAX];
    unsigned long base;

    memset(&mb, 0, sizeof(mb));
    memset(name, 0, sizeof(name));

    if (copy_from_user(&mb, (void __user *)arg1, sizeof(mb)))
        return -EFAULT;

    if (mb.pid <= 0)
        return -EINVAL;

    if (!mb.name_ptr)
        return -EINVAL;

    /* 从用户空间复制模块名 */
    if (strncpy_from_user(name, (void __user *)mb.name_ptr,
                          sizeof(name) - 1) < 0)
        return -EFAULT;

    name[sizeof(name) - 1] = '\0';

    /* 获取模块基址 */
    base = get_module_base(mb.pid, name);

    /* 返回结果 */
    mb.result = base;

    if (copy_to_user((void __user *)arg1, &mb, sizeof(mb)))
        return -EFAULT;

    return 0;
}

/*
 * 处理获取模块BSS地址
 */
static long cmd_get_module_bss(unsigned long arg1)
{
    struct tear_module_base mb;
    char name[TEAR_MODULE_NAME_MAX];
    unsigned long bss;

    memset(&mb, 0, sizeof(mb));
    memset(name, 0, sizeof(name));

    if (copy_from_user(&mb, (void __user *)arg1, sizeof(mb)))
        return -EFAULT;

    if (mb.pid <= 0)
        return -EINVAL;

    if (!mb.name_ptr)
        return -EINVAL;

    if (strncpy_from_user(name, (void __user *)mb.name_ptr,
                          sizeof(name) - 1) < 0)
        return -EFAULT;

    name[sizeof(name) - 1] = '\0';

    bss = get_module_bss(mb.pid, name);

    mb.result = bss;

    if (copy_to_user((void __user *)arg1, &mb, sizeof(mb)))
        return -EFAULT;

    return 0;
}

/*
 * 处理查找PID（标准方式）
 */
static long cmd_find_pid(unsigned long arg1)
{
    struct tear_get_pid gp;
    char name[TEAR_CMDLINE_MAX_LEN];
    pid_t pid;

    memset(&gp, 0, sizeof(gp));
    memset(name, 0, sizeof(name));

    if (copy_from_user(&gp, (void __user *)arg1, sizeof(gp)))
        return -EFAULT;

    if (!gp.name_ptr)
        return -EINVAL;

    if (strncpy_from_user(name, (void __user *)gp.name_ptr,
                          sizeof(name) - 1) < 0)
        return -EFAULT;

    name[sizeof(name) - 1] = '\0';

    pid = find_pid_safe(name);

    gp.result = pid;
    tear_read_guard_set_pid(pid);

    if (copy_to_user((void __user *)arg1, &gp, sizeof(gp)))
        return -EFAULT;

    return 0;
}

/*
 * 处理查找PID（隐蔽/缓存方式）
 */
static long cmd_find_pid_stealth(unsigned long arg1)
{
    struct tear_get_pid gp;
    char name[TEAR_CMDLINE_MAX_LEN];
    pid_t pid;

    memset(&gp, 0, sizeof(gp));
    memset(name, 0, sizeof(name));

    if (copy_from_user(&gp, (void __user *)arg1, sizeof(gp)))
        return -EFAULT;

    if (!gp.name_ptr)
        return -EINVAL;

    if (strncpy_from_user(name, (void __user *)gp.name_ptr,
                          sizeof(name) - 1) < 0)
        return -EFAULT;

    name[sizeof(name) - 1] = '\0';

    pid = find_pid_stealth(name);

    gp.result = pid;
    tear_read_guard_set_pid(pid);

    if (copy_to_user((void __user *)arg1, &gp, sizeof(gp)))
        return -EFAULT;

    return 0;
}

/*
 * 处理通过名称查找PID
 */
static long cmd_find_pid_byname(unsigned long arg1)
{
    struct tear_get_pid gp;
    char name[TEAR_CMDLINE_MAX_LEN];
    pid_t pid;

    memset(&gp, 0, sizeof(gp));
    memset(name, 0, sizeof(name));

    if (copy_from_user(&gp, (void __user *)arg1, sizeof(gp)))
        return -EFAULT;

    if (!gp.name_ptr)
        return -EINVAL;

    if (strncpy_from_user(name, (void __user *)gp.name_ptr,
                          sizeof(name) - 1) < 0)
        return -EFAULT;

    name[sizeof(name) - 1] = '\0';

    pid = find_pid_by_name(name);

    gp.result = pid;
    tear_read_guard_set_pid(pid);

    if (copy_to_user((void __user *)arg1, &gp, sizeof(gp)))
        return -EFAULT;

    return 0;
}

/*
 * ============================================================================
 * 隐藏功能命令处理
 * ============================================================================
 */

/*
 * 处理文件隐藏
 */
static long cmd_hide_file(unsigned long arg1)
{
    struct tear_hide_file hf;
    char name[256];
    char parent_path[512];

    memset(&hf, 0, sizeof(hf));
    memset(name, 0, sizeof(name));
    memset(parent_path, 0, sizeof(parent_path));

    if (copy_from_user(&hf, (void __user *)arg1, sizeof(hf)))
        return -EFAULT;

    if (!hf.name_ptr)
        return -EINVAL;

    if (strncpy_from_user(name, (void __user *)hf.name_ptr,
                          sizeof(name) - 1) < 0)
        return -EFAULT;

    name[sizeof(name) - 1] = '\0';

    /* 父目录路径（可选） */
    if (hf.parent_path_ptr) {
        if (strncpy_from_user(parent_path, (void __user *)hf.parent_path_ptr,
                              sizeof(parent_path) - 1) < 0)
            parent_path[0] = '\0';
        parent_path[sizeof(parent_path) - 1] = '\0';
    }

    return tear_file_hide_add(name, parent_path[0] ? parent_path : NULL);
}

/*
 * 处理取消文件隐藏
 */
static long cmd_unhide_file(unsigned long arg1)
{
    struct tear_hide_file hf;
    char name[256];

    memset(&hf, 0, sizeof(hf));
    memset(name, 0, sizeof(name));

    if (copy_from_user(&hf, (void __user *)arg1, sizeof(hf)))
        return -EFAULT;

    if (!hf.name_ptr)
        return -EINVAL;

    if (strncpy_from_user(name, (void __user *)hf.name_ptr,
                          sizeof(name) - 1) < 0)
        return -EFAULT;

    name[sizeof(name) - 1] = '\0';

    return tear_file_hide_remove(name);
}

/*
 * 处理按PID隐藏进程
 */
static long cmd_hide_process(unsigned long arg1)
{
    struct tear_hide_proc hp;

    if (copy_from_user(&hp, (void __user *)arg1, sizeof(hp)))
        return -EFAULT;

    if (hp.pid <= 0)
        return -EINVAL;

    return tear_proc_hide_pid(hp.pid);
}

/*
 * 处理按名称隐藏进程
 */
static long cmd_hide_process_name(unsigned long arg1)
{
    struct tear_hide_proc hp;
    char name[TASK_COMM_LEN];

    memset(&hp, 0, sizeof(hp));
    memset(name, 0, sizeof(name));

    if (copy_from_user(&hp, (void __user *)arg1, sizeof(hp)))
        return -EFAULT;

    if (!hp.name_ptr)
        return -EINVAL;

    if (strncpy_from_user(name, (void __user *)hp.name_ptr,
                          sizeof(name) - 1) < 0)
        return -EFAULT;

    name[sizeof(name) - 1] = '\0';

    return tear_proc_hide_name(name);
}

/*
 * 处理取消进程隐藏
 */
static long cmd_unhide_process(unsigned long arg1)
{
    struct tear_hide_proc hp;

    if (copy_from_user(&hp, (void __user *)arg1, sizeof(hp)))
        return -EFAULT;

    if (hp.pid <= 0)
        return -EINVAL;

    return tear_proc_unhide_pid(hp.pid);
}

/*
 * 处理 SO 隐藏
 */
static long cmd_hide_so(unsigned long arg1)
{
    struct tear_hide_so hs;
    char name[128];

    memset(&hs, 0, sizeof(hs));
    memset(name, 0, sizeof(name));

    if (copy_from_user(&hs, (void __user *)arg1, sizeof(hs)))
        return -EFAULT;

    if (!hs.name_ptr)
        return -EINVAL;

    if (strncpy_from_user(name, (void __user *)hs.name_ptr,
                          sizeof(name) - 1) < 0)
        return -EFAULT;

    name[sizeof(name) - 1] = '\0';

    return tear_so_hide_set(name);
}

/*
 * 处理 Shadow Page 附加
 */
static long cmd_shadow_attach(unsigned long arg1)
{
    struct tear_shadow_attach sa;

    if (copy_from_user(&sa, (void __user *)arg1, sizeof(sa)))
        return -EFAULT;

    return tear_shadow_attach(sa.pid, sa.target_addr);
}

/*
 * 处理 Shadow Page 设置寄存器轮转
 */
static long cmd_shadow_set_rot(unsigned long arg1)
{
    struct tear_shadow_set_rot sr;

    if (copy_from_user(&sr, (void __user *)arg1, sizeof(sr)))
        return -EFAULT;

    return tear_shadow_set_rot(&sr);
}

/*
 * 处理 Shadow Page 获取状态
 */
static long cmd_shadow_get_status(unsigned long arg1)
{
    struct tear_shadow_status ss;

    if (tear_shadow_get_status(&ss) != 0)
        return -ENOENT;

    if (copy_to_user((void __user *)arg1, &ss, sizeof(ss)))
        return -EFAULT;

    return 0;
}

/*
 * 处理 Syscall 拦截
 */
static long cmd_sc_block(unsigned long arg1)
{
    struct tear_sc_block sb;

    if (copy_from_user(&sb, (void __user *)arg1, sizeof(sb)))
        return -EFAULT;

    return tear_sc_block_set(sb.pid);
}

/*
 * ============================================================================
 * 主命令分发器
 * ============================================================================
 */

/*
 * 处理来自钩子的命令
 */
long teargame_handle_command(unsigned long cmd, unsigned long arg1,
                             unsigned long arg2, unsigned long arg3)
{
    /* 处理魔数查询（不需要认证） */
    if (cmd == TEAR_CMD_MAGIC)
        return cmd_magic();

    /* 处理认证（不需要认证） */
    if (cmd == TEAR_CMD_AUTH)
        return cmd_auth(arg1);

    /*
     * 放宽模式（阶段1收紧后）:
     * 仅保留 READ_MEMORY 免认证，其他读取命令已恢复认证。
     */
    if (cmd == TEAR_CMD_READ_MEMORY)
        goto dispatch;

    /* 所有其他命令需要认证 */
    if (!tear_auth_is_verified()) {
        tear_debug("命令 0x%lx 被拒绝: 未认证\n", cmd);
        return -EPERM;
    }

    /* 分发命令 */
dispatch:
    switch (cmd) {
    /* 内存操作 */
    case TEAR_CMD_READ_MEMORY:
        return cmd_read_memory(arg1, arg2);

    case TEAR_CMD_READ_SAFE:
        return cmd_read_safe(arg1);

    case TEAR_CMD_READ_BATCH:
        return cmd_read_batch(arg1, arg2);

    case TEAR_CMD_READ_SCATTER:
        return cmd_read_scatter(arg1, arg2);

    case TEAR_CMD_WRITE_SAFE:
        return cmd_write_safe(arg1);

    case TEAR_CMD_WRITE_MEMORY:
        return cmd_write_memory(arg1);

    case TEAR_CMD_WRITE_BATCH:
        return cmd_write_batch(arg1);

    /* 触控操作 */
    case TEAR_CMD_TOUCH_INIT:
        return cmd_touch_init(arg1);

    case TEAR_CMD_TOUCH_DOWN:
        return cmd_touch_down(arg1);

    case TEAR_CMD_TOUCH_MOVE:
        return cmd_touch_move(arg1);

    case TEAR_CMD_TOUCH_UP:
        return cmd_touch_up(arg1);

    /* 进程操作 */
    case TEAR_CMD_GET_MODULE_BASE:
        return cmd_get_module_base(arg1);

    case TEAR_CMD_GET_MODULE_BSS:
        return cmd_get_module_bss(arg1);

    case TEAR_CMD_FIND_PID:
        return cmd_find_pid(arg1);

    case TEAR_CMD_FIND_PID_STEALTH:
        return cmd_find_pid_stealth(arg1);

    case TEAR_CMD_FIND_PID_BYNAME:
        return cmd_find_pid_byname(arg1);

    /* 隐藏功能 */
    case TEAR_CMD_HIDE_FILE:
        return cmd_hide_file(arg1);

    case TEAR_CMD_UNHIDE_FILE:
        return cmd_unhide_file(arg1);

    case TEAR_CMD_HIDE_PROCESS:
        return cmd_hide_process(arg1);

    case TEAR_CMD_HIDE_PROCESS_NAME:
        return cmd_hide_process_name(arg1);

    case TEAR_CMD_UNHIDE_PROCESS:
        return cmd_unhide_process(arg1);

    /* SO 隐藏 */
    case TEAR_CMD_HIDE_SO:
        return cmd_hide_so(arg1);

    case TEAR_CMD_UNHIDE_SO:
        tear_so_hide_clear();
        return 0;

    /* Shadow Page Hook */
    case TEAR_CMD_SHADOW_ATTACH:
        return cmd_shadow_attach(arg1);

    case TEAR_CMD_SHADOW_DETACH:
        tear_shadow_detach();
        return 0;

    case TEAR_CMD_SHADOW_SET_ROT:
        return cmd_shadow_set_rot(arg1);

    case TEAR_CMD_SHADOW_STATUS:
        return cmd_shadow_get_status(arg1);

    /* Syscall Block */
    case TEAR_CMD_SC_BLOCK:
        return cmd_sc_block(arg1);

    case TEAR_CMD_SC_UNBLOCK:
        tear_sc_block_set(0);
        return 0;

    /* Crash Log */
    case TEAR_CMD_CRASH_DUMP:
        return tear_crash_dump((struct tear_crash_dump __user *)arg1);

    case TEAR_CMD_CRASH_CLEAR:
        tear_crash_clear();
        return 0;

    default:
        tear_debug("未知命令: 0x%lx\n", cmd);
        return -EINVAL;
    }
}
