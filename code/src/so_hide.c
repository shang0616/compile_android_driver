// SPDX-License-Identifier: GPL-2.0
/*
 * TearGame SO 隐藏模块
 *
 * 通过 hook show_map / show_smap / show_numa_map
 * 从 /proc/self/maps、/proc/self/smaps 中隐藏指定 SO 的映射条目。
 * SO 本身功能不受影响，只是不在内存映射列表中显示。
 */

#include "teargame.h"
#include <linux/kprobes.h>
#include <linux/seq_file.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/fs.h>
#include <linux/dcache.h>
#include <linux/slab.h>

#if !defined(__nocfi)
#define __nocfi __attribute__((no_sanitize("cfi")))
#endif

static char target_so_name[128];

struct maps_hide_data {
    struct seq_file *m;
    size_t saved_count;
    bool hit;
};

static bool vma_matches_target(struct vm_area_struct *vma)
{
    struct file *file;
    char buf[256];
    char *path;

    if (!vma)
        return false;

    file = vma->vm_file;
    if (!file)
        return false;

    if (target_so_name[0] == '\0')
        return false;

    path = d_path(&file->f_path, buf, sizeof(buf));
    if (IS_ERR(path))
        return false;

    return strstr(path, target_so_name) != NULL;
}

/* ---- show_map entry / ret ---- */

static int __nocfi show_map_entry(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct maps_hide_data *d = (void *)ri->data;

#ifdef CONFIG_ARM64
    d->m = (struct seq_file *)regs->regs[0];
#else
    d->m = (struct seq_file *)regs->regs[0];
#endif

    if (!d->m || target_so_name[0] == '\0') {
        d->hit = false;
        return 0;
    }

    d->saved_count = d->m->count;
    d->hit = false;
    return 0;
}

static int __nocfi show_map_ret(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct maps_hide_data *d = (void *)ri->data;
    struct vm_area_struct *vma;

    if (!d->hit && d->m && target_so_name[0] != '\0') {
#ifdef CONFIG_ARM64
        vma = (struct vm_area_struct *)regs->regs[1];
#else
        vma = (struct vm_area_struct *)regs->regs[1];
#endif
        if (vma_matches_target(vma)) {
            d->m->count = d->saved_count;
            d->hit = true;
        }
    }
    return 0;
}

/* ---- kretprobe 定义 ---- */

static struct kretprobe krp_show_map = {
    .handler       = show_map_ret,
    .entry_handler = show_map_entry,
    .data_size     = sizeof(struct maps_hide_data),
    .maxactive     = 20,
    .kp.symbol_name = "show_map",
};

static struct kretprobe krp_show_smap = {
    .handler       = show_map_ret,
    .entry_handler = show_map_entry,
    .data_size     = sizeof(struct maps_hide_data),
    .maxactive     = 20,
    .kp.symbol_name = "show_smap",
};

static struct kretprobe krp_show_numa_map = {
    .handler       = show_map_ret,
    .entry_handler = show_map_entry,
    .data_size     = sizeof(struct maps_hide_data),
    .maxactive     = 20,
    .kp.symbol_name = "show_numa_map",
};

static bool hooks_installed;

/* ---- 公共 API ---- */

int tear_so_hide_set(const char *name)
{
    if (!name || !name[0])
        return -EINVAL;

    strncpy(target_so_name, name, sizeof(target_so_name) - 1);
    target_so_name[sizeof(target_so_name) - 1] = '\0';
    tear_debug("so_hide: target set to '%s'\n", target_so_name);
    return 0;
}

void tear_so_hide_clear(void)
{
    target_so_name[0] = '\0';
    tear_debug("so_hide: target cleared\n");
}

int teargame_so_hide_init(void)
{
    int ret;

    target_so_name[0] = '\0';

    ret = register_kretprobe(&krp_show_map);
    if (ret < 0) {
        tear_warn("so_hide: show_map hook failed: %d\n", ret);
        return ret;
    }

    ret = register_kretprobe(&krp_show_smap);
    if (ret < 0) {
        tear_warn("so_hide: show_smap hook failed: %d\n", ret);
        goto err_smap;
    }

    ret = register_kretprobe(&krp_show_numa_map);
    if (ret < 0) {
        /* numa_map 在新内核可能不存在，非致命 */
        tear_debug("so_hide: show_numa_map hook failed: %d (non-fatal)\n", ret);
        krp_show_numa_map.kp.addr = NULL;
    }

    hooks_installed = true;
    tear_debug("so_hide: hooks installed (show_map + show_smap)\n");
    return 0;

err_smap:
    unregister_kretprobe(&krp_show_map);
    return ret;
}

void teargame_so_hide_exit(void)
{
    if (!hooks_installed)
        return;

    if (krp_show_numa_map.kp.addr)
        unregister_kretprobe(&krp_show_numa_map);
    unregister_kretprobe(&krp_show_smap);
    unregister_kretprobe(&krp_show_map);
    hooks_installed = false;

    tear_debug("so_hide: hooks removed\n");
}
