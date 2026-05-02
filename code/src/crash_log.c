/* SPDX-License-Identifier: GPL-2.0 */
/*
 * TearGame Crash Logger
 *
 * All logging is done via printk (visible in dmesg/ramoops/pstore).
 * The ring buffer can be read by userspace via the CRASH_LOG command.
 * No VFS symbols needed — GKI-safe.
 *
 * Userspace writes to /storage/emulated/0/ by reading via command:
 *   send CRASH_DUMP command → receive entries → write to file
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/string.h>

#include "teargame.h"

#define CRASH_LOG_RING_SIZE   64
#define CRASH_LOG_MSG_LEN    192
#define CRASH_LOG_CTX_LEN     32

struct crash_entry {
    u64             timestamp_ns;
    int             cpu;
    pid_t           pid;
    char            comm[TASK_COMM_LEN];
    char            ctx[CRASH_LOG_CTX_LEN];
    char            msg[CRASH_LOG_MSG_LEN];
    unsigned long   addr;
    unsigned long   extra;
    int             err_code;
    bool            is_fatal;
    bool            valid;
};

static struct crash_entry crash_ring[CRASH_LOG_RING_SIZE];
static atomic_t crash_write_idx = ATOMIC_INIT(0);
static atomic_t crash_total = ATOMIC_INIT(0);
static atomic_t crash_fatal = ATOMIC_INIT(0);
static spinlock_t crash_lock;

static bool log_initialized;

static const char *get_ctx_str(int ctx)
{
    switch (ctx) {
    case 0:  return "NORMAL";
    case 1:  return "KPROBE_ENTRY";
    case 2:  return "KPROBE_RET";
    case 3:  return "READ_SAFE";
    case 4:  return "WRITE_SAFE";
    case 5:  return "READ_MEM";
    case 6:  return "WRITE_MEM";
    case 7:  return "BATCH_READ";
    case 8:  return "SHADOW_HOOK";
    case 9:  return "SC_BLOCK";
    case 10: return "TOUCH";
    case 11: return "INIT";
    case 12: return "EXIT";
    case 13: return "PANIC";
    default: return "UNKNOWN";
    }
}

static void crash_store(int ctx, unsigned long addr, unsigned long extra,
                        int err_code, bool fatal, const char *fmt, va_list args)
{
    struct crash_entry *e;
    unsigned long flags;
    int idx;

    idx = atomic_inc_return(&crash_write_idx) % CRASH_LOG_RING_SIZE;

    spin_lock_irqsave(&crash_lock, flags);

    e = &crash_ring[idx];
    e->timestamp_ns = ktime_get_ns();
    e->cpu = raw_smp_processor_id();
    e->pid = current->pid;
    e->addr = addr;
    e->extra = extra;
    e->err_code = err_code;
    e->is_fatal = fatal;
    e->valid = true;

    get_task_comm(e->comm, current);

    snprintf(e->ctx, sizeof(e->ctx), "%s", get_ctx_str(ctx));

    vsnprintf(e->msg, sizeof(e->msg), fmt, args);

    spin_unlock_irqrestore(&crash_lock, flags);

    atomic_inc(&crash_total);
    if (fatal)
        atomic_inc(&crash_fatal);

    /* Always print fatal errors to kernel log (reaches pstore/ramoops) */
    if (fatal) {
        pr_err("teargame: %s [%s] CPU%d %s(%d) addr=0x%lx extra=0x%lx err=%d %s\n",
               "FATAL", e->ctx, e->cpu, e->comm, e->pid,
               e->addr, e->extra, e->err_code, e->msg);
    } else {
        pr_warn("teargame: %s [%s] CPU%d %s(%d) addr=0x%lx extra=0x%lx err=%d %s\n",
                "ERROR", e->ctx, e->cpu, e->comm, e->pid,
                e->addr, e->extra, e->err_code, e->msg);
    }
}

void tear_crash_log(int ctx, const char *fmt, ...)
{
    va_list args;

    if (!log_initialized)
        return;

    va_start(args, fmt);
    crash_store(ctx, 0, 0, 0, false, fmt, args);
    va_end(args);
}

void tear_crash_log_addr(int ctx, unsigned long addr, unsigned long extra,
                         int err_code, const char *fmt, ...)
{
    va_list args;

    if (!log_initialized)
        return;

    va_start(args, fmt);
    crash_store(ctx, addr, extra, err_code, false, fmt, args);
    va_end(args);
}

void tear_crash_log_fatal(int ctx, unsigned long addr, unsigned long extra,
                          int err_code, const char *fmt, ...)
{
    va_list args;

    if (!log_initialized)
        return;

    va_start(args, fmt);
    crash_store(ctx, addr, extra, err_code, true, fmt, args);
    va_end(args);
}

/*
 * Export to userspace: read up to 'count' entries starting from 'start_idx'.
 * Returns number of entries actually written. Userspace should loop until
 * it gets 0 (no more entries).
 */
int tear_crash_dump(struct tear_crash_dump __user *dump)
{
    struct tear_crash_dump kd;
    int i, total, start;
    unsigned long flags;

    if (!log_initialized)
        return -ENODEV;

    if (copy_from_user(&kd, dump, sizeof(kd)))
        return -EFAULT;

    if (kd.count == 0 || kd.count > CRASH_LOG_RING_SIZE)
        return -EINVAL;
    if (!kd.entries)
        return -EINVAL;

    spin_lock_irqsave(&crash_lock, flags);
    total = min_t(int, crash_total.counter, CRASH_LOG_RING_SIZE);
    spin_unlock_irqrestore(&crash_lock, flags);

    start = kd.start_index;

    for (i = 0; i < kd.count; i++) {
        struct tear_crash_entry uk;
        struct crash_entry *e;
        int idx;

        if (start + i >= total)
            break;

        idx = (crash_write_idx.counter - total + start + i + CRASH_LOG_RING_SIZE)
              % CRASH_LOG_RING_SIZE;

        spin_lock_irqsave(&crash_lock, flags);
        e = &crash_ring[idx];
        if (!e->valid) {
            spin_unlock_irqrestore(&crash_lock, flags);
            continue;
        }
        uk.timestamp_ns = e->timestamp_ns;
        uk.cpu = e->cpu;
        uk.pid = e->pid;
        uk.addr = e->addr;
        uk.extra = e->extra;
        uk.err_code = e->err_code;
        uk.is_fatal = e->is_fatal;
        strncpy(uk.comm, e->comm, sizeof(uk.comm));
        uk.comm[sizeof(uk.comm) - 1] = '\0';
        strncpy(uk.ctx, e->ctx, sizeof(uk.ctx));
        uk.ctx[sizeof(uk.ctx) - 1] = '\0';
        strncpy(uk.msg, e->msg, sizeof(uk.msg));
        uk.msg[sizeof(uk.msg) - 1] = '\0';
        spin_unlock_irqrestore(&crash_lock, flags);

        if (copy_to_user((void __user *)(unsigned long)kd.entries + i * sizeof(uk), &uk, sizeof(uk)))
            return -EFAULT;
    }

    kd.total_entries = total;
    kd.fatal_count = atomic_read(&crash_fatal);
    kd.returned = min_t(int, i, kd.count);

    if (copy_to_user(dump, &kd, sizeof(kd)))
        return -EFAULT;

    return kd.returned;
}

void tear_crash_clear(void)
{
    unsigned long flags;

    spin_lock_irqsave(&crash_lock, flags);
    memset(crash_ring, 0, sizeof(crash_ring));
    atomic_set(&crash_write_idx, 0);
    atomic_set(&crash_total, 0);
    atomic_set(&crash_fatal, 0);
    spin_unlock_irqrestore(&crash_lock, flags);
}

int teargame_crash_log_init(void)
{
    spin_lock_init(&crash_lock);
    memset(crash_ring, 0, sizeof(crash_ring));
    log_initialized = true;

    tear_crash_log(11, "Crash logger initialized (printk+ringbuf, VFS-free)");

    return 0;
}

void teargame_crash_log_exit(void)
{
    log_initialized = false;
}
