// SPDX-License-Identifier: GPL-2.0
/*
 * TearGame 增强进程隐藏模块 — /proc 路径过滤
 *
 * 在现有 getdents64 目录隐藏基础上，追加路径级过滤：
 *   - newfstatat  (stat/fstat 检测进程存在)
 *   - faccessat   (access 检测进程存在)
 *   - chdir       (cd 进入/proc/PID)
 *
 * 三路覆盖防止通过直接路径访问绕过目录隐藏。
 */

#include "teargame.h"
#include "teargame_stealth.h"
#include <linux/kprobes.h>
#include <linux/namei.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/slab.h>

#define UNTAG_ADDR(addr) ((addr) & 0x00ffffffffffffffUL)

#if !defined(__nocfi)
#define __nocfi __attribute__((no_sanitize("cfi")))
#endif

typedef unsigned long (*kallsyms_lookup_name_t)(const char *name);
typedef long (*copy_from_user_nofault_t)(void *dst, const void __user *src, size_t size);
typedef long (*copy_to_user_nofault_t)(void __user *dst, const void *src, size_t size);

static kallsyms_lookup_name_t p_kallsyms_lookup_name;
static copy_from_user_nofault_t p_copy_from_user_nofault;
static copy_to_user_nofault_t p_copy_to_user_nofault;

struct path_hook_data {
    int hit;
    pid_t pid;          /* entry 快照，整个生命周期只用这一份 */
};

/* ---- 符号解析 ---- */

static unsigned long resolve_kln(void)
{
    struct kprobe kp = { .symbol_name = "kallsyms_lookup_name" };
    unsigned long addr;
    if (register_kprobe(&kp) < 0) return 0;
    addr = (unsigned long)kp.addr;
    unregister_kprobe(&kp);
    return addr;
}

static int resolve_nofault(void)
{
    unsigned long addr = resolve_kln();
    if (!addr) return -1;
    p_kallsyms_lookup_name = (kallsyms_lookup_name_t)addr;
    p_copy_from_user_nofault = (copy_from_user_nofault_t)
        p_kallsyms_lookup_name("copy_from_user_nofault");
    p_copy_to_user_nofault = (copy_to_user_nofault_t)
        p_kallsyms_lookup_name("copy_to_user_nofault");
    if (!p_copy_from_user_nofault || !p_copy_to_user_nofault)
        return -1;
    return 0;
}

/* ---- 安全路径读取 ---- */

static int __nocfi safe_read_path(char *dst, unsigned long user_addr, size_t max_len)
{
    long ret;
    void __user *ptr = (void __user *)UNTAG_ADDR(user_addr);
    if (!p_copy_from_user_nofault) return -1;
    ret = p_copy_from_user_nofault(dst, ptr, max_len);
    if (ret != 0) return -1;
    dst[max_len - 1] = '\0';
    return 0;
}

/* ---- 路径匹配 ---- */

static int is_proc_pid_path(const char *path, pid_t *out_pid)
{
    const char *p;
    pid_t pid = 0;

    if (strncmp(path, "/proc/", 6) != 0)
        return 0;

    p = path + 6;
    if (*p < '0' || *p > '9')
        return 0;

    while (*p >= '0' && *p <= '9') {
        pid = pid * 10 + (*p - '0');
        if (pid > PID_MAX_LIMIT) return 0;
        p++;
    }

    if (*p != '\0' && *p != '/')
        return 0;

    *out_pid = pid;
    return 1;
}

/* ---- 通用入口（stat/faccessat/chdir 共用）---- */

static int path_entry_common(struct kretprobe_instance *ri, struct pt_regs *regs,
                             int path_reg_index)
{
    struct path_hook_data *d = (void *)ri->data;
    unsigned long path_ptr;
    char path[128];

    d->hit = 0;

#ifdef CONFIG_ARM64
    {
        struct pt_regs *user_regs;
        if (regs->regs[0] > 0xffffff0000000000UL) {
            user_regs = (struct pt_regs *)regs->regs[0];
            path_ptr = user_regs->regs[path_reg_index];
        } else {
            path_ptr = regs->regs[path_reg_index];
        }
    }
#else
    path_ptr = regs->regs[path_reg_index];
#endif

    if (safe_read_path(path, path_ptr, sizeof(path)) != 0)
        return 0;

    if (is_proc_pid_path(path, &d->pid)) {
        if (tear_proc_is_hidden(d->pid))
            d->hit = 1;
    }
    return 0;
}

static int path_ret(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct path_hook_data *d = (void *)ri->data;
    if (d->hit) {
#ifdef CONFIG_ARM64
        regs->regs[0] = -ENOENT;
#else
        regs->regs[0] = -ENOENT;
#endif
    }
    return 0;
}

/* ---- newfstatat: path 在 regs[1] ---- */
static int newfstatat_entry(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    return path_entry_common(ri, regs, 1);
}

/* ---- faccessat: path 在 regs[1] ---- */
static int faccessat_entry(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    return path_entry_common(ri, regs, 1);
}

/* ---- chdir: path 在 regs[0] ---- */
static int chdir_entry(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    return path_entry_common(ri, regs, 0);
}

/* ---- kretprobe 定义 ---- */

static struct kretprobe krp_newfstatat = {
    .handler       = path_ret,
    .entry_handler = newfstatat_entry,
    .data_size     = sizeof(struct path_hook_data),
    .maxactive     = 20,
    .kp.symbol_name = "__arm64_sys_newfstatat",
};

static struct kretprobe krp_faccessat = {
    .handler       = path_ret,
    .entry_handler = faccessat_entry,
    .data_size     = sizeof(struct path_hook_data),
    .maxactive     = 20,
    .kp.symbol_name = "__arm64_sys_faccessat",
};

static struct kretprobe krp_chdir = {
    .handler       = path_ret,
    .entry_handler = chdir_entry,
    .data_size     = sizeof(struct path_hook_data),
    .maxactive     = 5,
    .kp.symbol_name = "__arm64_sys_chdir",
};

static bool hooks_installed;
static bool faccessat_hook_installed;

static int register_faccessat_hook_with_fallback(void)
{
    static const char *const candidates[] = {
        "__arm64_sys_faccessat",
        "__arm64_sys_faccessat2",
    };
    int i;
    int ret = -ENOENT;

    for (i = 0; i < ARRAY_SIZE(candidates); i++) {
        krp_faccessat.kp.symbol_name = candidates[i];
        ret = register_kretprobe(&krp_faccessat);
        if (ret == 0) {
            faccessat_hook_installed = true;
            tear_debug("proc_path_hide: faccessat hook symbol=%s\n", candidates[i]);
            return 0;
        }
    }

    return ret;
}

/* ---- 公共 API ---- */

int teargame_proc_path_hide_init(void)
{
    int ret;

    if (resolve_nofault() != 0) {
        tear_warn("proc_path_hide: resolve nofault symbols failed\n");
        return -ENOENT;
    }

    ret = register_kretprobe(&krp_newfstatat);
    if (ret < 0) {
        tear_warn("proc_path_hide: newfstatat hook failed: %d\n", ret);
        return ret;
    }

    ret = register_faccessat_hook_with_fallback();
    if (ret < 0) {
        tear_warn("proc_path_hide: faccessat hook failed: %d\n", ret);
        unregister_kretprobe(&krp_newfstatat);
        return ret;
    }

    ret = register_kretprobe(&krp_chdir);
    if (ret < 0) {
        tear_warn("proc_path_hide: chdir hook failed: %d\n", ret);
        unregister_kretprobe(&krp_newfstatat);
        if (faccessat_hook_installed) {
            unregister_kretprobe(&krp_faccessat);
            faccessat_hook_installed = false;
        }
        return ret;
    }

    hooks_installed = true;
    tear_debug("proc_path_hide: hooks installed (newfstatat + faccessat + chdir)\n");
    return 0;
}

void teargame_proc_path_hide_exit(void)
{
    if (!hooks_installed)
        return;

    unregister_kretprobe(&krp_chdir);
    if (faccessat_hook_installed) {
        unregister_kretprobe(&krp_faccessat);
        faccessat_hook_installed = false;
    }
    unregister_kretprobe(&krp_newfstatat);
    hooks_installed = false;

    tear_debug("proc_path_hide: hooks removed\n");
}
