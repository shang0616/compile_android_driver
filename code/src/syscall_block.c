// SPDX-License-Identifier: GPL-2.0
/*
 * TearGame Syscall 拦截模块 (Dream Driver 移植)
 *
 * 通过 kprobe hook do_mmap，拦截目标进程的特定 mmap 请求。
 * 当目标进程尝试以 PROT_READ|PROT_WRITE(0x3) 映射 800KB-950KB 区间的
 * 匿名内存且 fd=0 时，直接返回 0 阻止调用。
 *
 * 功能默认编译但禁用 (TEAR_FEATURE_SYSCALL_BLOCK=0)。
 */

#include "teargame.h"
#include <linux/kprobes.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <asm/ptrace.h>

#if TEAR_FEATURE_SYSCALL_BLOCK

static bool sc_enabled;
module_param_named(sc_block_enable, sc_enabled, bool, 0600);
MODULE_PARM_DESC(sc_block_enable, "Enable syscall blocking (0=off, 1=on)");

static int sc_block_pid;

static inline void sc_block_set_pid(int pid)
{
    WRITE_ONCE(sc_block_pid, pid);
}

static int sc_mmap_pre(struct kprobe *p, struct pt_regs *regs)
{
    int pid;
    unsigned long aligned;

    (void)p;

    if (!sc_enabled)
        return 0;

    pid = READ_ONCE(sc_block_pid);
    if (!pid)
        return 0;
    if (current->tgid != pid)
        return 0;

    /* fd != 0 → 不是匿名映射 */
    if (regs->regs[0])
        return 0;

    /* prot != PROT_READ|PROT_WRITE */
    if (regs->regs[3] != 0x3)
        return 0;

    /* 只拦截 800KB~950KB 区间的映射 */
    aligned = PAGE_ALIGN(regs->regs[2]);
    if (aligned < (800UL * 1024) || aligned > (950UL * 1024))
        return 0;

    /* 如果有返回地址指针，清零 */
    if (regs->regs[7])
        *(unsigned long *)regs->regs[7] = 0;

    /* 阻止调用 */
    regs->regs[0] = 0;
    regs->pc      = regs->regs[30];
    return 1;
}

static struct kprobe sc_kp_mmap = {
    .symbol_name = "do_mmap",
    .pre_handler = sc_mmap_pre,
};

static bool hook_installed;

/* ---- 公共 API ---- */

int tear_sc_block_set(pid_t pid)
{
    sc_block_set_pid(pid);
    tear_debug("syscall_block: target pid=%d\n", pid);
    return 0;
}

int teargame_syscall_block_init(void)
{
    int ret;

    if (!sc_enabled) {
        tear_debug("syscall_block: disabled (sc_block_enable=0), skipping init\n");
        return 0;
    }

    ret = register_kprobe(&sc_kp_mmap);
    if (ret < 0) {
        tear_warn("syscall_block: do_mmap kprobe failed: %d\n", ret);
        return ret;
    }

    hook_installed = true;
    sc_block_pid = 0;
    tear_debug("syscall_block: hook installed\n");
    return 0;
}

void teargame_syscall_block_exit(void)
{
    if (!hook_installed)
        return;

    sc_block_set_pid(0);
    synchronize_rcu();
    unregister_kprobe(&sc_kp_mmap);
    hook_installed = false;
    tear_debug("syscall_block: hook removed\n");
}

#else /* !TEAR_FEATURE_SYSCALL_BLOCK */

int teargame_syscall_block_init(void) { return 0; }
void teargame_syscall_block_exit(void) { }

int tear_sc_block_set(pid_t pid)
{
    (void)pid;
    return -ENOSYS;
}

#endif /* TEAR_FEATURE_SYSCALL_BLOCK */
