// SPDX-License-Identifier: GPL-2.0
/*
 * TearGame kernel module - main entry
 *
 * Features:
 * - Cross-process memory read/write
 * - Process lookup and module base address
 * - Virtual touch input device
 * - Module stealth capabilities
 * - Enhanced /proc path filtering
 * - SO hiding
 * - Shadow Page instruction hook (optional)
 * - Syscall blocking (optional)
 */

#include "teargame.h"
#include <linux/jump_label.h>

struct static_key_false mte_async_mode = STATIC_KEY_FALSE_INIT;

/*
 * Global state
 */
struct tear_global_state g_state = {
    .memory_init = false,
    .touch_init = false,
    .hook_init = false,
    .stealth_init = false,
    .cache_init = false,
};

/*
 * Module init
 */
static int __init teargame_init(void)
{
    int ret;

    /* Auth */
    tear_auth_init();

    /* Memory subsystem */
    ret = teargame_memory_init();
    if (ret < 0) {
        tear_err("mem init failed: %d\n", ret);
        goto err_memory;
    }
    g_state.memory_init = true;

    /* Safe memory */
    ret = teargame_memory_safe_init();
    if (ret < 0) {
        tear_err("safe mem init failed: %d\n", ret);
        goto err_memory_safe;
    }

    /* Batch memory */
    ret = teargame_memory_batch_init();
    if (ret < 0) {
        tear_err("batch mem init failed: %d\n", ret);
        goto err_memory_batch;
    }

    /* Anti-debug */
    ret = teargame_antidbg_init();
    if (ret < 0) {
        tear_err("antidbg init failed: %d\n", ret);
        goto err_antidbg;
    }

    /* File hide */
    ret = teargame_file_hide_init();
    if (ret < 0) {
        tear_err("file hide init failed: %d\n", ret);
        goto err_file_hide;
    }

    /* Process hide (getdents64) */
    ret = teargame_proc_hide_init();
    if (ret < 0) {
        tear_err("proc hide init failed: %d\n", ret);
        goto err_proc_hide;
    }

    /* P0: Enhanced /proc path filtering (newfstatat + faccessat + chdir) */
    ret = teargame_proc_path_hide_init();
    if (ret < 0) {
        tear_err("proc path hide init failed: %d\n", ret);
        goto err_proc_path_hide;
    }

    /* P1: SO hiding (show_map/show_smap) */
    ret = teargame_so_hide_init();
    if (ret < 0) {
        tear_err("so hide init failed: %d\n", ret);
        goto err_so_hide;
    }

    /* P2: Shadow Page hook (disabled by default) */
    ret = teargame_shadow_hook_init();
    if (ret < 0) {
        tear_err("shadow hook init failed: %d\n", ret);
        goto err_shadow;
    }

    /* P3: Syscall block (disabled by default) */
    ret = teargame_syscall_block_init();
    if (ret < 0) {
        tear_err("syscall block init failed: %d\n", ret);
        goto err_sc_block;
    }

    /* Process cache */
    ret = tear_cache_init();
    if (ret < 0) {
        tear_err("cache init failed: %d\n", ret);
        goto err_cache;
    }
    g_state.cache_init = true;

    /* Touch device */
    ret = teargame_touch_module_init();
    if (ret < 0) {
        tear_err("touch init failed: %d\n", ret);
        goto err_touch;
    }
    g_state.touch_init = true;

    /* Stealth */
    ret = teargame_stealth_init();
    if (ret < 0) {
        tear_err("stealth init failed: %d\n", ret);
        goto err_stealth;
    }
    g_state.stealth_init = true;

    /* Prctl hook */
    ret = teargame_hook_init();
    if (ret < 0) {
        tear_err("hook init failed: %d\n", ret);
        goto err_hook;
    }
    g_state.hook_init = true;

#if TEAR_FEATURE_STEALTH
    teargame_stealth_hide();
#endif

    return 0;

err_hook:
    teargame_stealth_cleanup();
err_stealth:
    teargame_touch_module_exit();
err_touch:
    tear_cache_cleanup();
err_cache:
    teargame_syscall_block_exit();
err_sc_block:
    teargame_shadow_hook_exit();
err_shadow:
    teargame_so_hide_exit();
err_so_hide:
    teargame_proc_path_hide_exit();
err_proc_path_hide:
    teargame_proc_hide_exit();
err_proc_hide:
    teargame_file_hide_exit();
err_file_hide:
    teargame_antidbg_exit();
err_antidbg:
    teargame_memory_batch_exit();
err_memory_batch:
    teargame_memory_safe_exit();
err_memory_safe:
    teargame_memory_exit();
err_memory:
    tear_auth_cleanup();

    tear_err("Module init FAILED!\n");
    return ret;
}

/*
 * Module exit
 */
static void __exit teargame_exit(void)
{
    tear_info("Module unloading...\n");

    if (g_state.stealth_init) {
        teargame_stealth_show();
    }

    tear_auth_cleanup();
    tear_info("Auth cleaned\n");

    if (g_state.hook_init) {
        teargame_hook_exit();
        tear_info("Hook removed\n");
    }

    if (g_state.touch_init) {
        teargame_touch_module_exit();
        tear_info("Touch cleaned\n");
    }

    if (g_state.cache_init) {
        tear_cache_cleanup();
        tear_info("Cache cleaned\n");
    }

    if (g_state.stealth_init) {
        teargame_stealth_cleanup();
        tear_info("Stealth cleaned\n");
    }

    /* P3: Syscall block */
    teargame_syscall_block_exit();
    tear_info("SC block cleaned\n");

    /* P2: Shadow hook */
    teargame_shadow_hook_exit();
    tear_info("Shadow cleaned\n");

    /* P1: SO hide */
    teargame_so_hide_exit();
    tear_info("SO hide cleaned\n");

    /* P0: Proc path hide */
    teargame_proc_path_hide_exit();
    tear_info("Proc path cleaned\n");

    /* Proc hide */
    teargame_proc_hide_exit();
    tear_info("Proc hide cleaned\n");

    /* File hide */
    teargame_file_hide_exit();
    tear_info("File hide cleaned\n");

    /* Anti-debug */
    teargame_antidbg_exit();
    tear_info("Antidbg cleaned\n");

    /* Batch memory */
    teargame_memory_batch_exit();
    tear_info("Batch mem cleaned\n");

    /* Memory subsystem */
    teargame_memory_safe_exit();
    if (g_state.memory_init) {
        teargame_memory_exit();
    }
    tear_info("Memory cleaned\n");

    tear_info("Module unloaded\n");
}

module_init(teargame_init);
module_exit(teargame_exit);

MODULE_LICENSE(TEARGAME_LICENSE);
MODULE_AUTHOR(TEARGAME_AUTHOR);
MODULE_DESCRIPTION(TEARGAME_DESC);
MODULE_VERSION(TEARGAME_VERSION);

#ifdef CONFIG_ARM64
MODULE_INFO(arch, "arm64");
#endif
