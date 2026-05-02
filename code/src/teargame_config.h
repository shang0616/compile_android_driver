/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Configuration Parameters
 */

#ifndef _TEARGAME_CONFIG_H
#define _TEARGAME_CONFIG_H

/*
 * ============================================================================
 * Build-time Identity Randomization Seed
 * Auto-generated from __TIME__; override with make randseed=0xHEX.
 * Used to shuffle: magic multiplier, log prefix, device names, etc.
 * ============================================================================
 */
#ifdef TEAR_BUILD_RANDSEED_OVERRIDE
  #define TEAR_BUILD_RANDSEED  TEAR_BUILD_RANDSEED_OVERRIDE
#else
  #define TEAR_BUILD_RANDSEED  \
    ((((__TIME__[0]-'0')*10+(__TIME__[1]-'0'))*3600U +  \
      ((__TIME__[3]-'0')*10+(__TIME__[4]-'0'))*60U   +  \
      ((__TIME__[6]-'0')*10+(__TIME__[7]-'0')))       \
     * 0x9E3779B9U + 0x55AA55AAU)
#endif

/*
 * Derived random constants from build seed
 */
#define TEAR_RAND_U32(offset)  ((TEAR_BUILD_RANDSEED * 0x7FEB352DU + (offset) * 0x846CA68BU) ^ (TEAR_BUILD_RANDSEED >> 16))

/* Randomized log prefix - each build gets a unique 4-char tag based on __TIME__ */
#define TEAR_LOG_TAG_STR  TEAR_LOG_PREFIX

/*
 * Module identifiers visible in /proc/modules, /sys/module/
 * These are randomized per-build when TEAR_RANDOMIZE_MODULE_IDENTITY=1.
 */
#ifdef CONFIG_TEARGAME_RANDOMIZE
  #define TEAR_RANDOMIZE_MODULE_IDENTITY  1
#else
  #define TEAR_RANDOMIZE_MODULE_IDENTITY  1
#endif

#if TEAR_RANDOMIZE_MODULE_IDENTITY
  /*
   * Module name derived from __TIME__ digits for per-build uniqueness.
   * Format: "vfs_HHMM" where HHMM = build hour+minute.
   */
  #define TEARGAME_MODULE_NAME_STR  "vfs_0000"
  #define TEAR_LOG_PREFIX           "[0000]"

  /* Touch device name base */
  #define TEAR_TOUCH_DEVICE_NAME_BASE  "input_core_virtual"
#else
  #define TEARGAME_MODULE_NAME_STR  "sys_monitor"
  #define TEAR_LOG_PREFIX           "[TearGame]"
  #define TEAR_TOUCH_DEVICE_NAME_BASE  "sys_monitor_virtual_touch"
#endif

/* ============================================================================
 * Module Information
 * ============================================================================ */
#define TEARGAME_VERSION        "1.0.0"
#define TEARGAME_AUTHOR         "Google Inc."
#define TEARGAME_DESC           "Virtual Filesystem Cache Driver"
#define TEARGAME_LICENSE        "GPL"

/*
 * ============================================================================
 * Memory Operation Limits
 * ============================================================================
 */
/* Maximum size for single read/write operation (1MB) */
#define TEAR_MAX_RW_SIZE        (1 << 20)

/* Maximum size for safe read/write (256KB) */
#define TEAR_MAX_SAFE_RW_SIZE   (256 * 1024)

/* Minimum valid address (skip NULL page) */
#define TEAR_MIN_VALID_ADDR     0x1000UL

/* Maximum scatter/gather entries per request */
#define TEAR_MAX_SCATTER_ENTRIES 64

/*
 * ============================================================================
 * Process Cache Configuration
 * ============================================================================
 */
/* Number of hash buckets for process cache */
#ifdef CONFIG_TEARGAME_CACHE_BUCKETS
  #define TEAR_CACHE_BUCKETS    CONFIG_TEARGAME_CACHE_BUCKETS
#else
  #define TEAR_CACHE_BUCKETS    256
#endif

/* Maximum cached process entries */
#define TEAR_CACHE_MAX_ENTRIES  2048

/* Cache refresh interval in jiffies (500ms) */
#define TEAR_CACHE_REFRESH_JIFFIES  (HZ / 2)

/* LRU expiry time in jiffies (30 seconds) */
#define TEAR_CACHE_LRU_EXPIRY   (30 * HZ)

/* Maximum command line length to cache */
#define TEAR_CMDLINE_MAX_LEN    256

/* Maximum module name length */
#define TEAR_MODULE_NAME_MAX    256

/*
 * ============================================================================
 * Touch Device Configuration
 * ============================================================================
 */
/* Number of multi-touch slots */
#ifdef CONFIG_TEARGAME_TOUCH_SLOTS
  #define TEAR_TOUCH_MAX_SLOTS  CONFIG_TEARGAME_TOUCH_SLOTS
#else
  #define TEAR_TOUCH_MAX_SLOTS  10
#endif

/* Default touch screen dimensions */
#define TEAR_TOUCH_DEFAULT_WIDTH    1080
#define TEAR_TOUCH_DEFAULT_HEIGHT   2400

/* Touch pressure range */
#define TEAR_TOUCH_MIN_PRESSURE     0
#define TEAR_TOUCH_MAX_PRESSURE     255
#define TEAR_TOUCH_DEFAULT_PRESSURE 128

/* Touch major/minor axis */
#define TEAR_TOUCH_MIN_MAJOR        0
#define TEAR_TOUCH_MAX_MAJOR        255
#define TEAR_TOUCH_DEFAULT_MAJOR    10

/* Touch device name */
#define TEAR_TOUCH_DEVICE_NAME      TEAR_TOUCH_DEVICE_NAME_BASE

/*
 * ============================================================================
 * Authentication Configuration
 * ============================================================================
 */
/* Magic number for identification - randomized per build */
#define TEAR_MAGIC              TEAR_RAND_U32(30)

/* Authentication time window (seconds) */
#define TEAR_AUTH_TIME_WINDOW   60

/* Maximum authentication attempts before lockout */
#define TEAR_AUTH_MAX_ATTEMPTS  5

/* Lockout duration (seconds) */
#define TEAR_AUTH_LOCKOUT_SEC   300

/* Hash seed for key generation - must match userspace */
#define TEAR_AUTH_HASH_SEED     0x1B2C3D4E

/* Key length (characters) */
#define TEAR_AUTH_KEY_LEN       16

/*
 * ============================================================================
 * Hook Configuration
 * ============================================================================
 */
/* Maximum active kretprobe instances */
#define TEAR_KRETPROBE_MAXACTIVE    64

/* Hooked syscall symbol name */
#define TEAR_HOOK_PRCTL_SYMBOL      "__arm64_sys_prctl"

/* Hooked kallsyms symbol name */
#define TEAR_HOOK_SSHOW_SYMBOL      "s_show"

/*
 * ============================================================================
 * Performance Tuning
 * ============================================================================
 */
/* Enable page prefetching */
#define TEAR_ENABLE_PREFETCH        1

/* Prefetch stride (pages) */
#define TEAR_PREFETCH_STRIDE        4

/* Enable batch page table walks */
#define TEAR_ENABLE_BATCH_PTW       1

/* Batch size for page table walks */
#define TEAR_BATCH_PTW_SIZE         16

/* Use percpu cache for allocations */
#define TEAR_USE_PERCPU_CACHE       1

/*
 * ============================================================================
 * Page Table Cache Configuration
 * ============================================================================
 */
/* Enable page table walk cache */
#define TEAR_ENABLE_PTW_CACHE       1

/* Page table cache entries (per-CPU) */
#define TEAR_PTW_CACHE_SIZE         32

/* Cache expiry time (jiffies) - ~100ms */
#define TEAR_PTW_CACHE_EXPIRY       (HZ / 10)

/* Cache hit statistics (for debugging) */
#define TEAR_PTW_CACHE_STATS        0

/*
 * ============================================================================
 * Debug Configuration
 * ============================================================================
 */
#ifdef CONFIG_TEARGAME_DEBUG
  #define TEAR_DEBUG_ENABLED        1
#else
  #define TEAR_DEBUG_ENABLED        0
#endif

/* Touch debug (separate from main debug) */
#define TEAR_TOUCH_DEBUG_DEFAULT    0

/* Verbose memory operation logging */
#define TEAR_MEMORY_DEBUG           0

/* Log all command invocations */
#define TEAR_COMMAND_DEBUG          0

/*
 * ============================================================================
 * Error Codes (negative values)
 * ============================================================================
 */
#define TEAR_SUCCESS                0
#define TEAR_ERR_INVALID_ARG        (-EINVAL)
#define TEAR_ERR_NOT_AUTHORIZED     (-EPERM)
#define TEAR_ERR_NO_MEMORY          (-ENOMEM)
#define TEAR_ERR_FAULT              (-EFAULT)
#define TEAR_ERR_NOT_FOUND          (-ESRCH)
#define TEAR_ERR_IO                 (-EIO)
#define TEAR_ERR_BUSY               (-EBUSY)
#define TEAR_ERR_INVALID_CMD        (-ENOTTY)

/*
 * ============================================================================
 * Feature Flags
 * ============================================================================
 */
/* Enable stealth features */
#define TEAR_FEATURE_STEALTH        1

/* Enable virtual touch */
#define TEAR_FEATURE_TOUCH          0

/* Enable process cache */
#define TEAR_FEATURE_CACHE          1

/* Enable safe memory operations */
#define TEAR_FEATURE_SAFE_MEM       1

/* Enable huge page support */
#define TEAR_FEATURE_HUGEPAGE       1

/* Enable enhanced /proc path filtering (newfstatat + faccessat + chdir) */
#define TEAR_FEATURE_PROC_PATH_HIDE 1

/* Enable SO hiding via /proc/maps hooks (show_map/show_smap) */
#define TEAR_FEATURE_SO_HIDE        1

/* Shadow Page instruction-level hook (Dream Driver) */
#define TEAR_FEATURE_SHADOW_PAGE    1

/* Syscall blocking via do_mmap interception (Dream Driver) */
#define TEAR_FEATURE_SYSCALL_BLOCK  1

/*
 * ============================================================================
 * Security Check Configuration
 * ============================================================================
 */

/* Enable VMA permission check - detect no-read trap regions */
#define TEAR_SECURITY_CHECK_VMA         0

/* Enable lightweight trap-only detection (runs when CHECK_VMA=0).
 * Only catches definitive traps: no VM_READ, device mappings (VM_IO/VM_PFNMAP),
 * and permission revocation. Skips anonymous-RO / VM_DONTEXPAND / VM_DONTCOPY
 * that can false-positive on normal game memory. */
#define TEAR_SECURITY_CHECK_TRAP        1

/*
 * VMA check enforcement policy:
 * 1 = hard enforce (fail immediately on check failure)
 * 0 = soft check (log but continue, fall through to page table / fault check)
 */
#define TEAR_SECURITY_VMA_ENFORCE_READ  0
#define TEAR_SECURITY_VMA_ENFORCE_WRITE 1

/* Enable page fault detection - skip unmapped/swapped pages */
#define TEAR_SECURITY_CHECK_PRESENT     1

/* Enable PROT_NONE detection - detect permission trap pages */
#define TEAR_SECURITY_CHECK_PROTNONE    0

/* Skip guard pages - Guard Page detection */
#define TEAR_SECURITY_SKIP_GUARD        0

/* Skip no-read VMA */
#define TEAR_SECURITY_SKIP_NOREAD       0

/* Skip device mapped regions (VM_IO/VM_PFNMAP) */
#define TEAR_SECURITY_SKIP_DEVICE       0

/* Skip reserved pages (PageReserved) */
#define TEAR_SECURITY_SKIP_RESERVED     0

/* Strict mode: skip on any suspicious condition */
#define TEAR_SECURITY_STRICT_MODE       0

/* Return zero instead of error on security check failure */
#define TEAR_SECURITY_SILENT_FAIL       0

/*
 * ============================================================================
 * Read Path Stability / Access Control Configuration
 * ============================================================================
 */
/* Read failure log sampling interval (ms) */
#define TEAR_READ_FAIL_LOG_INTERVAL_MS      1000
/* Minimum new failures per sample interval to print summary */
#define TEAR_READ_FAIL_LOG_MIN_DELTA        16
/* Consecutive failure threshold to trigger short circuit breaker */
#define TEAR_READ_FAIL_CIRCUIT_THRESHOLD    48
/* Circuit breaker cooldown duration (ms) */
#define TEAR_READ_FAIL_CIRCUIT_COOLDOWN_MS  150

/*
 * Default off for compatibility; enable incrementally.
 */
/* Read PID whitelist: only allow the most recent find_pid target */
#define TEAR_SECURITY_ENFORCE_READ_PID_WHITELIST  0
/* Read secondary token verification (token via prctl 4th arg arg2) */
#define TEAR_SECURITY_ENFORCE_READ_TOKEN          0
/* Token time window (seconds) */
#define TEAR_SECURITY_TOKEN_WINDOW_SEC            30

/*
 * ============================================================================
 * Jitter Configuration
 * ============================================================================
 */
/* Jitter strength: 0=off, 1=light, 2=medium, 3=heavy */
#define TEAR_JITTER_LEVEL           2

/*
 * ============================================================================
 * Security Error Codes
 * ============================================================================
 */
#define TEAR_ERR_VMA_UNSAFE         (-1001)
#define TEAR_ERR_PAGE_FAULT         (-1002)
#define TEAR_ERR_TRAP_ADDR          (-1003)
#define TEAR_ERR_PROT_NONE          (-1004)
#define TEAR_ERR_NOT_PRESENT        (-1005)
#define TEAR_ERR_RESERVED_PAGE      (-1006)

#endif /* _TEARGAME_CONFIG_H */