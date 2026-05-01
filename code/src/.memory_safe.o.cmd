cmd_/home/shan/android-kernel/HOOK/src/memory_safe.o := clang -Wp,-MMD,/home/shan/android-kernel/HOOK/src/.memory_safe.o.d -nostdinc -isystem /home/shan/android-kernel/prebuilts-master/clang/host/linux-x86/clang-r416183b/lib64/clang/12.0.5/include -I/home/shan/android-kernel/common/arch/arm64/include -I./arch/arm64/include/generated -I/home/shan/android-kernel/common/include -I./include -I/home/shan/android-kernel/common/arch/arm64/include/uapi -I./arch/arm64/include/generated/uapi -I/home/shan/android-kernel/common/include/uapi -I./include/generated/uapi -include /home/shan/android-kernel/common/include/linux/kconfig.h -include /home/shan/android-kernel/common/include/linux/compiler_types.h -D__KERNEL__ -mlittle-endian -DKASAN_SHADOW_SCALE_SHIFT= -Qunused-arguments -fmacro-prefix-map=/home/shan/android-kernel/common/= -Wall -Wundef -Werror=strict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -fshort-wchar -fno-PIE -Werror=implicit-function-declaration -Werror=implicit-int -Werror=return-type -Wno-format-security -std=gnu89 --target=aarch64-linux-gnu --prefix=/usr/bin/aarch64-linux-gnu- --gcc-toolchain=/usr -fintegrated-as -Werror=unknown-warning-option -mgeneral-regs-only -DCONFIG_CC_HAS_K_CONSTRAINT=1 -Wno-psabi -fno-asynchronous-unwind-tables -fno-unwind-tables -mbranch-protection=pac-ret+leaf+bti -Wa,-march=armv8.5-a -DARM64_ASM_ARCH='"armv8.5-a"' -ffixed-x18 -DKASAN_SHADOW_SCALE_SHIFT= -fno-delete-null-pointer-checks -Wno-frame-address -Wno-address-of-packed-member -O2 -Wframe-larger-than=2048 -fstack-protector-strong -Werror -Wno-format-invalid-specifier -Wno-gnu -mno-global-merge -Wno-unused-const-variable -fno-omit-frame-pointer -fno-optimize-sibling-calls -ftrivial-auto-var-init=zero -enable-trivial-auto-var-init-zero-knowing-it-will-be-removed-from-clang -g -gdwarf-4 -fsanitize=shadow-call-stack -Wdeclaration-after-statement -Wvla -Wno-pointer-sign -Wno-array-bounds -fno-strict-overflow -fno-stack-check -Werror=date-time -Werror=incompatible-pointer-types -Wno-initializer-overrides -Wno-format -Wno-sign-compare -Wno-format-zero-length -Wno-pointer-to-enum-cast -Wno-tautological-constant-out-of-range-compare -Wno-enum-compare-conditional -Wno-enum-enum-conversion -I/home/shan/android-kernel/HOOK/include -DMODULE_NAME=\"vfs_core\" -O2 -fno-strict-aliasing -fno-common -fno-delete-null-pointer-checks  -fsanitize=array-bounds -fsanitize=local-bounds -fsanitize-undefined-trap-on-error  -DMODULE  -DKBUILD_BASENAME='"memory_safe"' -DKBUILD_MODNAME='"vfs_core"' -D__KBUILD_MODNAME=kmod_vfs_core -c -o /home/shan/android-kernel/HOOK/src/memory_safe.o /home/shan/android-kernel/HOOK/src/memory_safe.c

source_/home/shan/android-kernel/HOOK/src/memory_safe.o := /home/shan/android-kernel/HOOK/src/memory_safe.c

deps_/home/shan/android-kernel/HOOK/src/memory_safe.o := \
  /home/shan/android-kernel/common/include/linux/kconfig.h \
    $(wildcard include/config/cc/version/text.h) \
    $(wildcard include/config/cpu/big/endian.h) \
    $(wildcard include/config/booger.h) \
    $(wildcard include/config/foo.h) \
  /home/shan/android-kernel/common/include/linux/compiler_types.h \
    $(wildcard include/config/have/arch/compiler/h.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/cc/has/asm/inline.h) \
  /home/shan/android-kernel/common/include/linux/compiler_attributes.h \
  /home/shan/android-kernel/common/include/linux/compiler-clang.h \
    $(wildcard include/config/arch/use/builtin/bswap.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/compiler.h \
  /home/shan/android-kernel/HOOK/include/teargame.h \
  /home/shan/android-kernel/common/include/linux/module.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/sysfs.h) \
    $(wildcard include/config/modules/tree/lookup.h) \
    $(wildcard include/config/livepatch.h) \
    $(wildcard include/config/cfi/clang.h) \
    $(wildcard include/config/unused/symbols.h) \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/tracepoints.h) \
    $(wildcard include/config/tree/srcu.h) \
    $(wildcard include/config/bpf/events.h) \
    $(wildcard include/config/jump/label.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
    $(wildcard include/config/kprobes.h) \
    $(wildcard include/config/have/static/call/inline.h) \
    $(wildcard include/config/module/unload.h) \
    $(wildcard include/config/constructors.h) \
    $(wildcard include/config/function/error/injection.h) \
    $(wildcard include/config/retpoline.h) \
    $(wildcard include/config/module/sig.h) \
  /home/shan/android-kernel/common/include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  /home/shan/android-kernel/common/include/linux/types.h \
    $(wildcard include/config/have/uid16.h) \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /home/shan/android-kernel/common/include/uapi/linux/types.h \
  arch/arm64/include/generated/uapi/asm/types.h \
  /home/shan/android-kernel/common/include/uapi/asm-generic/types.h \
  /home/shan/android-kernel/common/include/asm-generic/int-ll64.h \
  /home/shan/android-kernel/common/include/uapi/asm-generic/int-ll64.h \
  /home/shan/android-kernel/common/arch/arm64/include/uapi/asm/bitsperlong.h \
  /home/shan/android-kernel/common/include/asm-generic/bitsperlong.h \
  /home/shan/android-kernel/common/include/uapi/asm-generic/bitsperlong.h \
  /home/shan/android-kernel/common/include/uapi/linux/posix_types.h \
  /home/shan/android-kernel/common/include/linux/stddef.h \
  /home/shan/android-kernel/common/include/uapi/linux/stddef.h \
  /home/shan/android-kernel/common/arch/arm64/include/uapi/asm/posix_types.h \
  /home/shan/android-kernel/common/include/uapi/asm-generic/posix_types.h \
  /home/shan/android-kernel/common/include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  /home/shan/android-kernel/common/include/linux/const.h \
  /home/shan/android-kernel/common/include/vdso/const.h \
  /home/shan/android-kernel/common/include/uapi/linux/const.h \
  /home/shan/android-kernel/common/include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/atomic/sleep.h) \
    $(wildcard include/config/preempt/rt.h) \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/panic/timeout.h) \
  /home/shan/android-kernel/common/include/linux/limits.h \
  /home/shan/android-kernel/common/include/uapi/linux/limits.h \
  /home/shan/android-kernel/common/include/vdso/limits.h \
  /home/shan/android-kernel/common/include/linux/linkage.h \
    $(wildcard include/config/arch/use/sym/annotations.h) \
  /home/shan/android-kernel/common/include/linux/stringify.h \
  /home/shan/android-kernel/common/include/linux/export.h \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/module/rel/crcs.h) \
    $(wildcard include/config/have/arch/prel32/relocations.h) \
    $(wildcard include/config/trim/unused/ksyms.h) \
  /home/shan/android-kernel/common/include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/stack/validation.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/rwonce.h \
    $(wildcard include/config/lto.h) \
    $(wildcard include/config/as/has/ldapr.h) \
  /home/shan/android-kernel/common/include/asm-generic/rwonce.h \
  /home/shan/android-kernel/common/include/linux/kasan-checks.h \
    $(wildcard include/config/kasan/generic.h) \
    $(wildcard include/config/kasan/sw/tags.h) \
  /home/shan/android-kernel/common/include/linux/kcsan-checks.h \
    $(wildcard include/config/kcsan.h) \
    $(wildcard include/config/kcsan/ignore/atomics.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/linkage.h \
    $(wildcard include/config/arm64/bti/kernel.h) \
  /home/shan/android-kernel/common/include/linux/bitops.h \
  /home/shan/android-kernel/common/include/linux/bits.h \
  /home/shan/android-kernel/common/include/vdso/bits.h \
  /home/shan/android-kernel/common/include/linux/build_bug.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/bitops.h \
  /home/shan/android-kernel/common/include/asm-generic/bitops/builtin-__ffs.h \
  /home/shan/android-kernel/common/include/asm-generic/bitops/builtin-ffs.h \
  /home/shan/android-kernel/common/include/asm-generic/bitops/builtin-__fls.h \
  /home/shan/android-kernel/common/include/asm-generic/bitops/builtin-fls.h \
  /home/shan/android-kernel/common/include/asm-generic/bitops/ffz.h \
  /home/shan/android-kernel/common/include/asm-generic/bitops/fls64.h \
  /home/shan/android-kernel/common/include/asm-generic/bitops/find.h \
    $(wildcard include/config/generic/find/first/bit.h) \
  /home/shan/android-kernel/common/include/asm-generic/bitops/sched.h \
  /home/shan/android-kernel/common/include/asm-generic/bitops/hweight.h \
  /home/shan/android-kernel/common/include/asm-generic/bitops/arch_hweight.h \
  /home/shan/android-kernel/common/include/asm-generic/bitops/const_hweight.h \
  /home/shan/android-kernel/common/include/asm-generic/bitops/atomic.h \
  /home/shan/android-kernel/common/include/linux/atomic.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/atomic.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/barrier.h \
    $(wildcard include/config/arm64/pseudo/nmi.h) \
  /home/shan/android-kernel/common/include/asm-generic/barrier.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/cmpxchg.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/lse.h \
    $(wildcard include/config/arm64/lse/atomics.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/atomic_ll_sc.h \
    $(wildcard include/config/cc/has/k/constraint.h) \
  /home/shan/android-kernel/common/include/linux/jump_label.h \
    $(wildcard include/config/have/arch/jump/label/relative.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/jump_label.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/insn.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/alternative.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/alternative-macros.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/cpucaps.h \
  /home/shan/android-kernel/common/include/linux/init.h \
    $(wildcard include/config/strict/kernel/rwx.h) \
    $(wildcard include/config/strict/module/rwx.h) \
    $(wildcard include/config/lto/clang.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/atomic_lse.h \
  /home/shan/android-kernel/common/include/linux/atomic-arch-fallback.h \
    $(wildcard include/config/generic/atomic64.h) \
  /home/shan/android-kernel/common/include/asm-generic/atomic-instrumented.h \
  /home/shan/android-kernel/common/include/linux/instrumented.h \
  /home/shan/android-kernel/common/include/asm-generic/atomic-long.h \
  /home/shan/android-kernel/common/include/asm-generic/bitops/lock.h \
  /home/shan/android-kernel/common/include/asm-generic/bitops/non-atomic.h \
  /home/shan/android-kernel/common/include/asm-generic/bitops/le.h \
  /home/shan/android-kernel/common/arch/arm64/include/uapi/asm/byteorder.h \
  /home/shan/android-kernel/common/include/linux/byteorder/little_endian.h \
  /home/shan/android-kernel/common/include/uapi/linux/byteorder/little_endian.h \
  /home/shan/android-kernel/common/include/linux/swab.h \
  /home/shan/android-kernel/common/include/uapi/linux/swab.h \
  arch/arm64/include/generated/uapi/asm/swab.h \
  /home/shan/android-kernel/common/include/uapi/asm-generic/swab.h \
  /home/shan/android-kernel/common/include/linux/byteorder/generic.h \
  /home/shan/android-kernel/common/include/asm-generic/bitops/ext2-atomic-setbit.h \
  /home/shan/android-kernel/common/include/linux/kstrtox.h \
  /home/shan/android-kernel/common/include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  /home/shan/android-kernel/common/include/linux/minmax.h \
  /home/shan/android-kernel/common/include/linux/typecheck.h \
  /home/shan/android-kernel/common/include/linux/printk.h \
    $(wildcard include/config/message/loglevel/default.h) \
    $(wildcard include/config/console/loglevel/default.h) \
    $(wildcard include/config/console/loglevel/quiet.h) \
    $(wildcard include/config/early/printk.h) \
    $(wildcard include/config/printk/nmi.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
    $(wildcard include/config/dynamic/debug/core.h) \
  /home/shan/android-kernel/common/include/linux/kern_levels.h \
  /home/shan/android-kernel/common/include/linux/ratelimit_types.h \
  /home/shan/android-kernel/common/include/uapi/linux/param.h \
  /home/shan/android-kernel/common/arch/arm64/include/uapi/asm/param.h \
  /home/shan/android-kernel/common/include/asm-generic/param.h \
    $(wildcard include/config/hz.h) \
  /home/shan/android-kernel/common/include/uapi/asm-generic/param.h \
  /home/shan/android-kernel/common/include/linux/spinlock_types.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/spinlock_types.h \
  /home/shan/android-kernel/common/include/asm-generic/qspinlock_types.h \
    $(wildcard include/config/nr/cpus.h) \
  /home/shan/android-kernel/common/include/asm-generic/qrwlock_types.h \
  /home/shan/android-kernel/common/include/linux/lockdep_types.h \
    $(wildcard include/config/prove/raw/lock/nesting.h) \
    $(wildcard include/config/preempt/lock.h) \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
  /home/shan/android-kernel/common/include/linux/rwlock_types.h \
  arch/arm64/include/generated/asm/div64.h \
  /home/shan/android-kernel/common/include/asm-generic/div64.h \
  /home/shan/android-kernel/common/include/uapi/linux/kernel.h \
  /home/shan/android-kernel/common/include/uapi/linux/sysinfo.h \
  /home/shan/android-kernel/common/include/linux/stat.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/stat.h \
    $(wildcard include/config/compat.h) \
  arch/arm64/include/generated/uapi/asm/stat.h \
  /home/shan/android-kernel/common/include/uapi/asm-generic/stat.h \
  /home/shan/android-kernel/common/include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
    $(wildcard include/config/posix/timers.h) \
  /home/shan/android-kernel/common/include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/cache.h \
    $(wildcard include/config/kasan/hw/tags.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/cputype.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/sysreg.h \
    $(wildcard include/config/broken/gas/inst.h) \
    $(wildcard include/config/arm64/pa/bits/52.h) \
    $(wildcard include/config/arm64/4k/pages.h) \
    $(wildcard include/config/arm64/16k/pages.h) \
    $(wildcard include/config/arm64/64k/pages.h) \
  /home/shan/android-kernel/common/include/linux/kasan-tags.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/mte-def.h \
  /home/shan/android-kernel/common/include/linux/kasan-enabled.h \
    $(wildcard include/config/kasan.h) \
  /home/shan/android-kernel/common/include/linux/static_key.h \
  /home/shan/android-kernel/common/include/linux/math64.h \
    $(wildcard include/config/arch/supports/int128.h) \
  /home/shan/android-kernel/common/include/vdso/math64.h \
  /home/shan/android-kernel/common/include/linux/time64.h \
  /home/shan/android-kernel/common/include/vdso/time64.h \
  /home/shan/android-kernel/common/include/uapi/linux/time.h \
  /home/shan/android-kernel/common/include/uapi/linux/time_types.h \
  /home/shan/android-kernel/common/include/linux/time32.h \
  /home/shan/android-kernel/common/include/linux/timex.h \
  /home/shan/android-kernel/common/include/uapi/linux/timex.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/timex.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/arch_timer.h \
    $(wildcard include/config/arm/arch/timer/ool/workaround.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/hwcap.h \
  /home/shan/android-kernel/common/arch/arm64/include/uapi/asm/hwcap.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/cpufeature.h \
    $(wildcard include/config/arm64/sw/ttbr0/pan.h) \
    $(wildcard include/config/arm64/sve.h) \
    $(wildcard include/config/arm64/cnp.h) \
    $(wildcard include/config/arm64/ptr/auth.h) \
    $(wildcard include/config/arm64/mte.h) \
    $(wildcard include/config/arm64/debug/priority/masking.h) \
    $(wildcard include/config/arm64/bti.h) \
    $(wildcard include/config/arm64/tlb/range.h) \
    $(wildcard include/config/arm64/pa/bits.h) \
    $(wildcard include/config/arm64/hw/afdbm.h) \
    $(wildcard include/config/arm64/amu/extn.h) \
  /home/shan/android-kernel/common/include/linux/bug.h \
    $(wildcard include/config/bug/on/data/corruption.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/bug.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/asm-bug.h \
    $(wildcard include/config/debug/bugverbose.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/brk-imm.h \
  /home/shan/android-kernel/common/include/asm-generic/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
  /home/shan/android-kernel/common/include/linux/instrumentation.h \
    $(wildcard include/config/debug/entry.h) \
  /home/shan/android-kernel/common/include/linux/smp.h \
    $(wildcard include/config/up/late/init.h) \
    $(wildcard include/config/debug/preempt.h) \
  /home/shan/android-kernel/common/include/linux/errno.h \
  /home/shan/android-kernel/common/include/uapi/linux/errno.h \
  arch/arm64/include/generated/uapi/asm/errno.h \
  /home/shan/android-kernel/common/include/uapi/asm-generic/errno.h \
  /home/shan/android-kernel/common/include/uapi/asm-generic/errno-base.h \
  /home/shan/android-kernel/common/include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
  /home/shan/android-kernel/common/include/linux/threads.h \
    $(wildcard include/config/base/small.h) \
  /home/shan/android-kernel/common/include/linux/bitmap.h \
  /home/shan/android-kernel/common/include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
    $(wildcard include/config/fortify/source.h) \
  /home/shan/android-kernel/common/include/uapi/linux/string.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/string.h \
    $(wildcard include/config/arch/has/uaccess/flushcache.h) \
  /home/shan/android-kernel/common/include/linux/smp_types.h \
  /home/shan/android-kernel/common/include/linux/llist.h \
    $(wildcard include/config/arch/have/nmi/safe/cmpxchg.h) \
  /home/shan/android-kernel/common/include/linux/preempt.h \
    $(wildcard include/config/preempt/count.h) \
    $(wildcard include/config/trace/preempt/toggle.h) \
    $(wildcard include/config/preemption.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/preempt.h \
  /home/shan/android-kernel/common/include/linux/thread_info.h \
    $(wildcard include/config/thread/info/in/task.h) \
    $(wildcard include/config/have/arch/within/stack/frames.h) \
    $(wildcard include/config/hardened/usercopy.h) \
  /home/shan/android-kernel/common/include/linux/restart_block.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/current.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/thread_info.h \
    $(wildcard include/config/shadow/call/stack.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/memory.h \
    $(wildcard include/config/arm64/va/bits.h) \
    $(wildcard include/config/kasan/shadow/offset.h) \
    $(wildcard include/config/vmap/stack.h) \
    $(wildcard include/config/debug/virtual.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/efi.h) \
    $(wildcard include/config/arm/gic/v3/its.h) \
  /home/shan/android-kernel/common/include/linux/sizes.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/page-def.h \
    $(wildcard include/config/arm64/page/shift.h) \
  /home/shan/android-kernel/common/include/linux/mmdebug.h \
    $(wildcard include/config/debug/vm.h) \
    $(wildcard include/config/debug/vm/pgflags.h) \
  /home/shan/android-kernel/common/include/asm-generic/memory_model.h \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/sparsemem.h) \
  /home/shan/android-kernel/common/include/linux/pfn.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/stack_pointer.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/smp.h \
    $(wildcard include/config/arm64/acpi/parking/protocol.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/percpu.h \
  /home/shan/android-kernel/common/include/asm-generic/percpu.h \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  /home/shan/android-kernel/common/include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
    $(wildcard include/config/amd/mem/encrypt.h) \
  /home/shan/android-kernel/common/include/clocksource/arm_arch_timer.h \
    $(wildcard include/config/arm/arch/timer.h) \
  /home/shan/android-kernel/common/include/linux/timecounter.h \
  /home/shan/android-kernel/common/include/asm-generic/timex.h \
  /home/shan/android-kernel/common/include/vdso/time32.h \
  /home/shan/android-kernel/common/include/vdso/time.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/compat.h \
  /home/shan/android-kernel/common/include/asm-generic/compat.h \
    $(wildcard include/config/compat/for/u64/alignment.h) \
  /home/shan/android-kernel/common/include/linux/sched.h \
    $(wildcard include/config/virt/cpu/accounting/native.h) \
    $(wildcard include/config/sched/info.h) \
    $(wildcard include/config/schedstats.h) \
    $(wildcard include/config/fair/group/sched.h) \
    $(wildcard include/config/rt/group/sched.h) \
    $(wildcard include/config/rt/mutexes.h) \
    $(wildcard include/config/uclamp/task.h) \
    $(wildcard include/config/uclamp/buckets/count.h) \
    $(wildcard include/config/cgroup/sched.h) \
    $(wildcard include/config/blk/dev/io/trace.h) \
    $(wildcard include/config/preempt/rcu.h) \
    $(wildcard include/config/tasks/rcu.h) \
    $(wildcard include/config/tasks/trace/rcu.h) \
    $(wildcard include/config/psi.h) \
    $(wildcard include/config/memcg.h) \
    $(wildcard include/config/compat/brk.h) \
    $(wildcard include/config/cgroups.h) \
    $(wildcard include/config/blk/cgroup.h) \
    $(wildcard include/config/stackprotector.h) \
    $(wildcard include/config/arch/has/scaled/cputime.h) \
    $(wildcard include/config/cpu/freq/times.h) \
    $(wildcard include/config/virt/cpu/accounting/gen.h) \
    $(wildcard include/config/no/hz/full.h) \
    $(wildcard include/config/posix/cputimers.h) \
    $(wildcard include/config/posix/cpu/timers/task/work.h) \
    $(wildcard include/config/keys.h) \
    $(wildcard include/config/sysvipc.h) \
    $(wildcard include/config/detect/hung/task.h) \
    $(wildcard include/config/io/uring.h) \
    $(wildcard include/config/audit.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/debug/mutexes.h) \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/ubsan.h) \
    $(wildcard include/config/ubsan/trap.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/compaction.h) \
    $(wildcard include/config/task/xacct.h) \
    $(wildcard include/config/cpusets.h) \
    $(wildcard include/config/x86/cpu/resctrl.h) \
    $(wildcard include/config/futex.h) \
    $(wildcard include/config/perf/events.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/numa/balancing.h) \
    $(wildcard include/config/rseq.h) \
    $(wildcard include/config/task/delay/acct.h) \
    $(wildcard include/config/fault/injection.h) \
    $(wildcard include/config/latencytop.h) \
    $(wildcard include/config/kunit.h) \
    $(wildcard include/config/function/graph/tracer.h) \
    $(wildcard include/config/kcov.h) \
    $(wildcard include/config/uprobes.h) \
    $(wildcard include/config/bcache.h) \
    $(wildcard include/config/security.h) \
    $(wildcard include/config/gcc/plugin/stackleak.h) \
    $(wildcard include/config/x86/mce.h) \
    $(wildcard include/config/rt/softint/optimization.h) \
    $(wildcard include/config/arch/task/struct/on/stack.h) \
    $(wildcard include/config/debug/rseq.h) \
  /home/shan/android-kernel/common/include/uapi/linux/sched.h \
  /home/shan/android-kernel/common/include/linux/pid.h \
  /home/shan/android-kernel/common/include/linux/rculist.h \
    $(wildcard include/config/prove/rcu/list.h) \
  /home/shan/android-kernel/common/include/linux/rcupdate.h \
    $(wildcard include/config/tiny/rcu.h) \
    $(wildcard include/config/tasks/rcu/generic.h) \
    $(wildcard include/config/rcu/stall/common.h) \
    $(wildcard include/config/rcu/nocb/cpu.h) \
    $(wildcard include/config/tasks/rude/rcu.h) \
    $(wildcard include/config/tree/rcu.h) \
    $(wildcard include/config/debug/objects/rcu/head.h) \
    $(wildcard include/config/prove/rcu.h) \
    $(wildcard include/config/rcu/boost.h) \
    $(wildcard include/config/arch/weak/release/acquire.h) \
  /home/shan/android-kernel/common/include/linux/irqflags.h \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/irqflags.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/ptrace.h \
  /home/shan/android-kernel/common/arch/arm64/include/uapi/asm/ptrace.h \
  /home/shan/android-kernel/common/arch/arm64/include/uapi/asm/sve_context.h \
  /home/shan/android-kernel/common/include/linux/bottom_half.h \
  /home/shan/android-kernel/common/include/linux/lockdep.h \
    $(wildcard include/config/debug/locking/api/selftests.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/processor.h \
    $(wildcard include/config/kuser/helpers.h) \
    $(wildcard include/config/arm64/force/52bit.h) \
    $(wildcard include/config/have/hw/breakpoint.h) \
    $(wildcard include/config/arm64/tagged/addr/abi.h) \
  /home/shan/android-kernel/common/include/linux/android_vendor.h \
    $(wildcard include/config/android/vendor/oem/data.h) \
  /home/shan/android-kernel/common/include/vdso/processor.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/vdso/processor.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/hw_breakpoint.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/virt.h \
    $(wildcard include/config/kvm.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/sections.h \
  /home/shan/android-kernel/common/include/asm-generic/sections.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/kasan.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/mte-kasan.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/pgtable-types.h \
    $(wildcard include/config/pgtable/levels.h) \
  /home/shan/android-kernel/common/include/asm-generic/pgtable-nopud.h \
  /home/shan/android-kernel/common/include/asm-generic/pgtable-nop4d.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/pgtable-hwdef.h \
    $(wildcard include/config/arm64/cont/pte/shift.h) \
    $(wildcard include/config/arm64/cont/pmd/shift.h) \
    $(wildcard include/config/arm64/va/bits/52.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/pointer_auth.h \
  /home/shan/android-kernel/common/include/uapi/linux/prctl.h \
  /home/shan/android-kernel/common/include/linux/random.h \
    $(wildcard include/config/arch/random.h) \
  /home/shan/android-kernel/common/include/linux/once.h \
  /home/shan/android-kernel/common/include/uapi/linux/random.h \
  /home/shan/android-kernel/common/include/uapi/linux/ioctl.h \
  arch/arm64/include/generated/uapi/asm/ioctl.h \
  /home/shan/android-kernel/common/include/asm-generic/ioctl.h \
  /home/shan/android-kernel/common/include/uapi/asm-generic/ioctl.h \
  /home/shan/android-kernel/common/include/linux/irqnr.h \
  /home/shan/android-kernel/common/include/uapi/linux/irqnr.h \
  /home/shan/android-kernel/common/include/linux/prandom.h \
  /home/shan/android-kernel/common/include/linux/percpu.h \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
  /home/shan/android-kernel/common/include/linux/siphash.h \
    $(wildcard include/config/have/efficient/unaligned/access.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/archrandom.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/spectre.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/fpsimd.h \
  /home/shan/android-kernel/common/arch/arm64/include/uapi/asm/sigcontext.h \
  /home/shan/android-kernel/common/include/linux/rcutree.h \
  /home/shan/android-kernel/common/include/linux/wait.h \
  /home/shan/android-kernel/common/include/linux/spinlock.h \
  arch/arm64/include/generated/asm/mmiowb.h \
  /home/shan/android-kernel/common/include/asm-generic/mmiowb.h \
    $(wildcard include/config/mmiowb.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/spinlock.h \
  arch/arm64/include/generated/asm/qrwlock.h \
  /home/shan/android-kernel/common/include/asm-generic/qrwlock.h \
  arch/arm64/include/generated/asm/qspinlock.h \
  /home/shan/android-kernel/common/include/asm-generic/qspinlock.h \
  /home/shan/android-kernel/common/include/linux/rwlock.h \
    $(wildcard include/config/preempt.h) \
  /home/shan/android-kernel/common/include/linux/spinlock_api_smp.h \
    $(wildcard include/config/inline/spin/lock.h) \
    $(wildcard include/config/inline/spin/lock/bh.h) \
    $(wildcard include/config/inline/spin/lock/irq.h) \
    $(wildcard include/config/inline/spin/lock/irqsave.h) \
    $(wildcard include/config/inline/spin/trylock.h) \
    $(wildcard include/config/inline/spin/trylock/bh.h) \
    $(wildcard include/config/uninline/spin/unlock.h) \
    $(wildcard include/config/inline/spin/unlock/bh.h) \
    $(wildcard include/config/inline/spin/unlock/irq.h) \
    $(wildcard include/config/inline/spin/unlock/irqrestore.h) \
    $(wildcard include/config/generic/lockbreak.h) \
  /home/shan/android-kernel/common/include/linux/rwlock_api_smp.h \
    $(wildcard include/config/inline/read/lock.h) \
    $(wildcard include/config/inline/write/lock.h) \
    $(wildcard include/config/inline/read/lock/bh.h) \
    $(wildcard include/config/inline/write/lock/bh.h) \
    $(wildcard include/config/inline/read/lock/irq.h) \
    $(wildcard include/config/inline/write/lock/irq.h) \
    $(wildcard include/config/inline/read/lock/irqsave.h) \
    $(wildcard include/config/inline/write/lock/irqsave.h) \
    $(wildcard include/config/inline/read/trylock.h) \
    $(wildcard include/config/inline/write/trylock.h) \
    $(wildcard include/config/inline/read/unlock.h) \
    $(wildcard include/config/inline/write/unlock.h) \
    $(wildcard include/config/inline/read/unlock/bh.h) \
    $(wildcard include/config/inline/write/unlock/bh.h) \
    $(wildcard include/config/inline/read/unlock/irq.h) \
    $(wildcard include/config/inline/write/unlock/irq.h) \
    $(wildcard include/config/inline/read/unlock/irqrestore.h) \
    $(wildcard include/config/inline/write/unlock/irqrestore.h) \
  /home/shan/android-kernel/common/include/uapi/linux/wait.h \
  /home/shan/android-kernel/common/include/linux/refcount.h \
  /home/shan/android-kernel/common/include/linux/sem.h \
  /home/shan/android-kernel/common/include/uapi/linux/sem.h \
  /home/shan/android-kernel/common/include/linux/ipc.h \
  /home/shan/android-kernel/common/include/linux/uidgid.h \
    $(wildcard include/config/multiuser.h) \
    $(wildcard include/config/user/ns.h) \
  /home/shan/android-kernel/common/include/linux/highuid.h \
  /home/shan/android-kernel/common/include/linux/rhashtable-types.h \
  /home/shan/android-kernel/common/include/linux/mutex.h \
    $(wildcard include/config/mutex/spin/on/owner.h) \
  /home/shan/android-kernel/common/include/linux/osq_lock.h \
  /home/shan/android-kernel/common/include/linux/debug_locks.h \
  /home/shan/android-kernel/common/include/linux/workqueue.h \
    $(wildcard include/config/debug/objects/work.h) \
    $(wildcard include/config/freezer.h) \
    $(wildcard include/config/wq/watchdog.h) \
  /home/shan/android-kernel/common/include/linux/timer.h \
    $(wildcard include/config/debug/objects/timers.h) \
    $(wildcard include/config/no/hz/common.h) \
  /home/shan/android-kernel/common/include/linux/ktime.h \
  /home/shan/android-kernel/common/include/linux/jiffies.h \
  /home/shan/android-kernel/common/include/vdso/jiffies.h \
  include/generated/timeconst.h \
  /home/shan/android-kernel/common/include/vdso/ktime.h \
  /home/shan/android-kernel/common/include/linux/timekeeping.h \
  /home/shan/android-kernel/common/include/linux/timekeeping32.h \
  /home/shan/android-kernel/common/include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  /home/shan/android-kernel/common/include/linux/android_kabi.h \
    $(wildcard include/config/android/kabi/reserve.h) \
  /home/shan/android-kernel/common/include/uapi/linux/ipc.h \
  arch/arm64/include/generated/uapi/asm/ipcbuf.h \
  /home/shan/android-kernel/common/include/uapi/asm-generic/ipcbuf.h \
  arch/arm64/include/generated/uapi/asm/sembuf.h \
  /home/shan/android-kernel/common/include/uapi/asm-generic/sembuf.h \
  /home/shan/android-kernel/common/include/linux/shm.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/page.h \
  /home/shan/android-kernel/common/include/linux/personality.h \
  /home/shan/android-kernel/common/include/uapi/linux/personality.h \
  /home/shan/android-kernel/common/include/asm-generic/getorder.h \
  /home/shan/android-kernel/common/include/uapi/linux/shm.h \
  /home/shan/android-kernel/common/include/uapi/asm-generic/hugetlb_encode.h \
  arch/arm64/include/generated/uapi/asm/shmbuf.h \
  /home/shan/android-kernel/common/include/uapi/asm-generic/shmbuf.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/shmparam.h \
  /home/shan/android-kernel/common/include/asm-generic/shmparam.h \
  /home/shan/android-kernel/common/include/linux/plist.h \
    $(wildcard include/config/debug/plist.h) \
  /home/shan/android-kernel/common/include/linux/hrtimer.h \
    $(wildcard include/config/high/res/timers.h) \
    $(wildcard include/config/time/low/res.h) \
    $(wildcard include/config/timerfd.h) \
  /home/shan/android-kernel/common/include/linux/hrtimer_defs.h \
  /home/shan/android-kernel/common/include/linux/rbtree.h \
  /home/shan/android-kernel/common/include/linux/seqlock.h \
  /home/shan/android-kernel/common/include/linux/ww_mutex.h \
    $(wildcard include/config/debug/ww/mutex/slowpath.h) \
  /home/shan/android-kernel/common/include/linux/timerqueue.h \
  /home/shan/android-kernel/common/include/linux/seccomp.h \
    $(wildcard include/config/seccomp.h) \
    $(wildcard include/config/have/arch/seccomp/filter.h) \
    $(wildcard include/config/seccomp/filter.h) \
    $(wildcard include/config/checkpoint/restore.h) \
  /home/shan/android-kernel/common/include/uapi/linux/seccomp.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/seccomp.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/unistd.h \
  /home/shan/android-kernel/common/arch/arm64/include/uapi/asm/unistd.h \
  /home/shan/android-kernel/common/include/uapi/asm-generic/unistd.h \
  /home/shan/android-kernel/common/include/asm-generic/seccomp.h \
  /home/shan/android-kernel/common/include/uapi/linux/unistd.h \
  /home/shan/android-kernel/common/include/linux/nodemask.h \
    $(wildcard include/config/highmem.h) \
  /home/shan/android-kernel/common/include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
    $(wildcard include/config/numa/keep/meminfo.h) \
  /home/shan/android-kernel/common/include/linux/resource.h \
  /home/shan/android-kernel/common/include/uapi/linux/resource.h \
  arch/arm64/include/generated/uapi/asm/resource.h \
  /home/shan/android-kernel/common/include/asm-generic/resource.h \
  /home/shan/android-kernel/common/include/uapi/asm-generic/resource.h \
  /home/shan/android-kernel/common/include/linux/latencytop.h \
  /home/shan/android-kernel/common/include/linux/sched/prio.h \
  /home/shan/android-kernel/common/include/linux/sched/types.h \
  /home/shan/android-kernel/common/include/linux/signal_types.h \
    $(wildcard include/config/old/sigaction.h) \
  /home/shan/android-kernel/common/include/uapi/linux/signal.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/signal.h \
  /home/shan/android-kernel/common/arch/arm64/include/uapi/asm/signal.h \
  /home/shan/android-kernel/common/include/asm-generic/signal.h \
  /home/shan/android-kernel/common/include/uapi/asm-generic/signal.h \
  /home/shan/android-kernel/common/include/uapi/asm-generic/signal-defs.h \
  arch/arm64/include/generated/uapi/asm/siginfo.h \
  /home/shan/android-kernel/common/include/uapi/asm-generic/siginfo.h \
  /home/shan/android-kernel/common/include/linux/mm_types_task.h \
    $(wildcard include/config/arch/want/batched/unmap/tlb/flush.h) \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/arch/enable/split/pmd/ptlock.h) \
  /home/shan/android-kernel/common/include/linux/task_io_accounting.h \
    $(wildcard include/config/task/io/accounting.h) \
  /home/shan/android-kernel/common/include/linux/posix-timers.h \
  /home/shan/android-kernel/common/include/linux/alarmtimer.h \
    $(wildcard include/config/rtc/class.h) \
  /home/shan/android-kernel/common/include/linux/task_work.h \
  /home/shan/android-kernel/common/include/uapi/linux/rseq.h \
  /home/shan/android-kernel/common/include/linux/kcsan.h \
  /home/shan/android-kernel/common/include/linux/sched/task_stack.h \
    $(wildcard include/config/stack/growsup.h) \
    $(wildcard include/config/debug/stack/usage.h) \
  /home/shan/android-kernel/common/include/uapi/linux/magic.h \
  /home/shan/android-kernel/common/include/uapi/linux/stat.h \
  /home/shan/android-kernel/common/include/linux/kmod.h \
  /home/shan/android-kernel/common/include/linux/umh.h \
  /home/shan/android-kernel/common/include/linux/gfp.h \
    $(wildcard include/config/cma.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/zone/device.h) \
    $(wildcard include/config/pm/sleep.h) \
    $(wildcard include/config/contig/alloc.h) \
  /home/shan/android-kernel/common/include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/memory/isolation.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/transparent/hugepage.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/page/extension.h) \
    $(wildcard include/config/deferred/struct/page/init.h) \
    $(wildcard include/config/have/memoryless/nodes.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/memory/hotremove.h) \
    $(wildcard include/config/have/arch/pfn/valid.h) \
    $(wildcard include/config/holes/in/zone.h) \
  /home/shan/android-kernel/common/include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  /home/shan/android-kernel/common/include/linux/page-flags-layout.h \
  include/generated/bounds.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/sparsemem.h \
  /home/shan/android-kernel/common/include/linux/mm_types.h \
    $(wildcard include/config/have/aligned/struct/page.h) \
    $(wildcard include/config/userfaultfd.h) \
    $(wildcard include/config/swap.h) \
    $(wildcard include/config/speculative/page/fault.h) \
    $(wildcard include/config/have/arch/compat/mmap/bases.h) \
    $(wildcard include/config/membarrier.h) \
    $(wildcard include/config/aio.h) \
    $(wildcard include/config/mmu/notifier.h) \
    $(wildcard include/config/iommu/support.h) \
  /home/shan/android-kernel/common/include/linux/auxvec.h \
  /home/shan/android-kernel/common/include/uapi/linux/auxvec.h \
  /home/shan/android-kernel/common/arch/arm64/include/uapi/asm/auxvec.h \
  /home/shan/android-kernel/common/include/linux/rwsem.h \
    $(wildcard include/config/rwsem/spin/on/owner.h) \
    $(wildcard include/config/debug/rwsems.h) \
  /home/shan/android-kernel/common/include/linux/err.h \
  /home/shan/android-kernel/common/include/linux/completion.h \
  /home/shan/android-kernel/common/include/linux/swait.h \
  /home/shan/android-kernel/common/include/linux/uprobes.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/uprobes.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/debug-monitors.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/esr.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/probes.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/mmu.h \
  /home/shan/android-kernel/common/include/linux/page-flags.h \
    $(wildcard include/config/arch/uses/pg/uncached.h) \
    $(wildcard include/config/memory/failure.h) \
    $(wildcard include/config/page/idle/flag.h) \
    $(wildcard include/config/thp/swap.h) \
    $(wildcard include/config/ksm.h) \
  /home/shan/android-kernel/common/include/linux/memory_hotplug.h \
    $(wildcard include/config/arch/has/add/pages.h) \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
    $(wildcard include/config/have/bootmem/info/node.h) \
  /home/shan/android-kernel/common/include/linux/notifier.h \
  /home/shan/android-kernel/common/include/linux/srcu.h \
    $(wildcard include/config/tiny/srcu.h) \
    $(wildcard include/config/srcu.h) \
  /home/shan/android-kernel/common/include/linux/rcu_segcblist.h \
  /home/shan/android-kernel/common/include/linux/srcutree.h \
  /home/shan/android-kernel/common/include/linux/rcu_node_tree.h \
    $(wildcard include/config/rcu/fanout.h) \
    $(wildcard include/config/rcu/fanout/leaf.h) \
  /home/shan/android-kernel/common/include/linux/topology.h \
    $(wildcard include/config/use/percpu/numa/node/id.h) \
    $(wildcard include/config/sched/smt.h) \
  /home/shan/android-kernel/common/include/linux/arch_topology.h \
    $(wildcard include/config/generic/arch/topology.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/topology.h \
  /home/shan/android-kernel/common/include/asm-generic/topology.h \
  /home/shan/android-kernel/common/include/linux/sysctl.h \
    $(wildcard include/config/sysctl.h) \
  /home/shan/android-kernel/common/include/uapi/linux/sysctl.h \
  /home/shan/android-kernel/common/include/linux/elf.h \
    $(wildcard include/config/arch/use/gnu/property.h) \
    $(wildcard include/config/arch/have/elf/prot.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/elf.h \
    $(wildcard include/config/compat/vdso.h) \
  arch/arm64/include/generated/asm/user.h \
  /home/shan/android-kernel/common/include/asm-generic/user.h \
  /home/shan/android-kernel/common/include/uapi/linux/elf.h \
  /home/shan/android-kernel/common/include/uapi/linux/elf-em.h \
  /home/shan/android-kernel/common/include/linux/fs.h \
    $(wildcard include/config/read/only/thp/for/fs.h) \
    $(wildcard include/config/fs/posix/acl.h) \
    $(wildcard include/config/cgroup/writeback.h) \
    $(wildcard include/config/ima.h) \
    $(wildcard include/config/file/locking.h) \
    $(wildcard include/config/fsnotify.h) \
    $(wildcard include/config/fs/encryption.h) \
    $(wildcard include/config/fs/verity.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/unicode.h) \
    $(wildcard include/config/quota.h) \
    $(wildcard include/config/fs/dax.h) \
    $(wildcard include/config/mandatory/file/locking.h) \
    $(wildcard include/config/migration.h) \
  /home/shan/android-kernel/common/include/linux/wait_bit.h \
  /home/shan/android-kernel/common/include/linux/kdev_t.h \
  /home/shan/android-kernel/common/include/uapi/linux/kdev_t.h \
  /home/shan/android-kernel/common/include/linux/dcache.h \
  /home/shan/android-kernel/common/include/linux/rculist_bl.h \
  /home/shan/android-kernel/common/include/linux/list_bl.h \
  /home/shan/android-kernel/common/include/linux/bit_spinlock.h \
  /home/shan/android-kernel/common/include/linux/lockref.h \
    $(wildcard include/config/arch/use/cmpxchg/lockref.h) \
  /home/shan/android-kernel/common/include/linux/stringhash.h \
    $(wildcard include/config/dcache/word/access.h) \
  /home/shan/android-kernel/common/include/linux/hash.h \
    $(wildcard include/config/have/arch/hash.h) \
  /home/shan/android-kernel/common/include/linux/path.h \
  /home/shan/android-kernel/common/include/linux/list_lru.h \
    $(wildcard include/config/memcg/kmem.h) \
  /home/shan/android-kernel/common/include/linux/shrinker.h \
  /home/shan/android-kernel/common/include/linux/radix-tree.h \
  /home/shan/android-kernel/common/include/linux/xarray.h \
    $(wildcard include/config/xarray/multi.h) \
  /home/shan/android-kernel/common/include/linux/local_lock.h \
  /home/shan/android-kernel/common/include/linux/local_lock_internal.h \
  /home/shan/android-kernel/common/include/linux/capability.h \
  /home/shan/android-kernel/common/include/uapi/linux/capability.h \
  /home/shan/android-kernel/common/include/linux/semaphore.h \
  /home/shan/android-kernel/common/include/linux/fcntl.h \
    $(wildcard include/config/arch/32bit/off/t.h) \
  /home/shan/android-kernel/common/include/uapi/linux/fcntl.h \
  /home/shan/android-kernel/common/arch/arm64/include/uapi/asm/fcntl.h \
  /home/shan/android-kernel/common/include/uapi/asm-generic/fcntl.h \
  /home/shan/android-kernel/common/include/uapi/linux/openat2.h \
  /home/shan/android-kernel/common/include/linux/migrate_mode.h \
  /home/shan/android-kernel/common/include/linux/percpu-rwsem.h \
  /home/shan/android-kernel/common/include/linux/rcuwait.h \
  /home/shan/android-kernel/common/include/linux/sched/signal.h \
    $(wildcard include/config/sched/autogroup.h) \
    $(wildcard include/config/bsd/process/acct.h) \
    $(wildcard include/config/taskstats.h) \
  /home/shan/android-kernel/common/include/linux/signal.h \
    $(wildcard include/config/proc/fs.h) \
  /home/shan/android-kernel/common/include/linux/sched/jobctl.h \
  /home/shan/android-kernel/common/include/linux/sched/task.h \
    $(wildcard include/config/have/exit/thread.h) \
    $(wildcard include/config/arch/wants/dynamic/task/struct.h) \
    $(wildcard include/config/have/arch/thread/struct/whitelist.h) \
  /home/shan/android-kernel/common/include/linux/uaccess.h \
    $(wildcard include/config/set/fs.h) \
  /home/shan/android-kernel/common/include/linux/fault-inject-usercopy.h \
    $(wildcard include/config/fault/injection/usercopy.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/uaccess.h \
    $(wildcard include/config/arm64/uao.h) \
    $(wildcard include/config/arm64/pan.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/kernel-pgtable.h \
    $(wildcard include/config/randomize/base.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/mte.h \
  /home/shan/android-kernel/common/include/linux/bitfield.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/extable.h \
    $(wildcard include/config/bpf/jit.h) \
  /home/shan/android-kernel/common/include/linux/cred.h \
    $(wildcard include/config/debug/credentials.h) \
  /home/shan/android-kernel/common/include/linux/key.h \
    $(wildcard include/config/key/notifications.h) \
    $(wildcard include/config/net.h) \
  /home/shan/android-kernel/common/include/linux/assoc_array.h \
    $(wildcard include/config/associative/array.h) \
  /home/shan/android-kernel/common/include/linux/sched/user.h \
    $(wildcard include/config/fanotify.h) \
    $(wildcard include/config/posix/mqueue.h) \
    $(wildcard include/config/bpf/syscall.h) \
    $(wildcard include/config/watch/queue.h) \
  /home/shan/android-kernel/common/include/linux/ratelimit.h \
  /home/shan/android-kernel/common/include/linux/rcu_sync.h \
  /home/shan/android-kernel/common/include/linux/delayed_call.h \
  /home/shan/android-kernel/common/include/linux/uuid.h \
  /home/shan/android-kernel/common/include/uapi/linux/uuid.h \
  /home/shan/android-kernel/common/include/linux/errseq.h \
  /home/shan/android-kernel/common/include/linux/ioprio.h \
  /home/shan/android-kernel/common/include/linux/sched/rt.h \
  /home/shan/android-kernel/common/include/linux/iocontext.h \
  /home/shan/android-kernel/common/include/linux/fs_types.h \
  /home/shan/android-kernel/common/include/uapi/linux/fs.h \
  /home/shan/android-kernel/common/include/linux/quota.h \
    $(wildcard include/config/quota/netlink/interface.h) \
  /home/shan/android-kernel/common/include/linux/percpu_counter.h \
  /home/shan/android-kernel/common/include/uapi/linux/dqblk_xfs.h \
  /home/shan/android-kernel/common/include/linux/dqblk_v1.h \
  /home/shan/android-kernel/common/include/linux/dqblk_v2.h \
  /home/shan/android-kernel/common/include/linux/dqblk_qtree.h \
  /home/shan/android-kernel/common/include/linux/projid.h \
  /home/shan/android-kernel/common/include/uapi/linux/quota.h \
  /home/shan/android-kernel/common/include/linux/nfs_fs_i.h \
  /home/shan/android-kernel/common/include/linux/kobject.h \
    $(wildcard include/config/uevent/helper.h) \
    $(wildcard include/config/debug/kobject/release.h) \
  /home/shan/android-kernel/common/include/linux/sysfs.h \
  /home/shan/android-kernel/common/include/linux/kernfs.h \
    $(wildcard include/config/kernfs.h) \
  /home/shan/android-kernel/common/include/linux/idr.h \
  /home/shan/android-kernel/common/include/linux/kobject_ns.h \
  /home/shan/android-kernel/common/include/linux/kref.h \
  /home/shan/android-kernel/common/include/linux/moduleparam.h \
    $(wildcard include/config/alpha.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/ppc64.h) \
  /home/shan/android-kernel/common/include/linux/rbtree_latch.h \
  /home/shan/android-kernel/common/include/linux/error-injection.h \
  /home/shan/android-kernel/common/include/asm-generic/error-injection.h \
  /home/shan/android-kernel/common/include/linux/tracepoint-defs.h \
  /home/shan/android-kernel/common/include/linux/static_call_types.h \
    $(wildcard include/config/have/static/call.h) \
  /home/shan/android-kernel/common/include/linux/cfi.h \
    $(wildcard include/config/cfi/clang/shadow.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/module.h \
    $(wildcard include/config/arm64/module/plts.h) \
    $(wildcard include/config/dynamic/ftrace.h) \
    $(wildcard include/config/arm64/erratum/843419.h) \
  /home/shan/android-kernel/common/include/asm-generic/module.h \
    $(wildcard include/config/have/mod/arch/specific.h) \
    $(wildcard include/config/modules/use/elf/rel.h) \
    $(wildcard include/config/modules/use/elf/rela.h) \
  /home/shan/android-kernel/common/include/linux/slab.h \
    $(wildcard include/config/debug/slab.h) \
    $(wildcard include/config/failslab.h) \
    $(wildcard include/config/have/hardened/usercopy/allocator.h) \
    $(wildcard include/config/slab.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/slob.h) \
  /home/shan/android-kernel/common/include/linux/overflow.h \
  /home/shan/android-kernel/common/include/linux/percpu-refcount.h \
  /home/shan/android-kernel/common/include/linux/kasan.h \
    $(wildcard include/config/kasan/stack.h) \
    $(wildcard include/config/kasan/vmalloc.h) \
    $(wildcard include/config/kasan/inline.h) \
  /home/shan/android-kernel/common/include/linux/vmalloc.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/vmalloc.h \
  /home/shan/android-kernel/common/include/linux/mm.h \
    $(wildcard include/config/have/arch/mmap/rnd/bits.h) \
    $(wildcard include/config/have/arch/mmap/rnd/compat/bits.h) \
    $(wildcard include/config/mem/soft/dirty.h) \
    $(wildcard include/config/arch/uses/high/vma/flags.h) \
    $(wildcard include/config/arch/has/pkeys.h) \
    $(wildcard include/config/ppc.h) \
    $(wildcard include/config/x86.h) \
    $(wildcard include/config/parisc.h) \
    $(wildcard include/config/sparc64.h) \
    $(wildcard include/config/arm64.h) \
    $(wildcard include/config/have/arch/userfaultfd/minor.h) \
    $(wildcard include/config/shmem.h) \
    $(wildcard include/config/dev/pagemap/ops.h) \
    $(wildcard include/config/device/private.h) \
    $(wildcard include/config/pci/p2pdma.h) \
    $(wildcard include/config/arch/has/pte/special.h) \
    $(wildcard include/config/arch/has/pte/devmap.h) \
    $(wildcard include/config/debug/vm/rb.h) \
    $(wildcard include/config/page/poisoning.h) \
    $(wildcard include/config/debug/pagealloc.h) \
    $(wildcard include/config/arch/has/set/direct/map.h) \
    $(wildcard include/config/hibernation.h) \
    $(wildcard include/config/hugetlbfs.h) \
    $(wildcard include/config/mapping/dirty/helpers.h) \
  /home/shan/android-kernel/common/include/linux/mmap_lock.h \
  /home/shan/android-kernel/common/include/linux/range.h \
  /home/shan/android-kernel/common/include/linux/page_ext.h \
    $(wildcard include/config/page/pinner.h) \
  /home/shan/android-kernel/common/include/linux/stacktrace.h \
    $(wildcard include/config/stacktrace.h) \
    $(wildcard include/config/arch/stackwalk.h) \
    $(wildcard include/config/have/reliable/stacktrace.h) \
  /home/shan/android-kernel/common/include/linux/stackdepot.h \
    $(wildcard include/config/stackdepot.h) \
  /home/shan/android-kernel/common/include/linux/page_ref.h \
    $(wildcard include/config/debug/page/ref.h) \
  /home/shan/android-kernel/common/include/linux/memremap.h \
  /home/shan/android-kernel/common/include/linux/ioport.h \
    $(wildcard include/config/io/strict/devmem.h) \
  /home/shan/android-kernel/common/include/linux/pgtable.h \
    $(wildcard include/config/highpte.h) \
    $(wildcard include/config/have/arch/transparent/hugepage/pud.h) \
    $(wildcard include/config/have/arch/soft/dirty.h) \
    $(wildcard include/config/arch/enable/thp/migration.h) \
    $(wildcard include/config/have/arch/huge/vmap.h) \
    $(wildcard include/config/x86/espfix64.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/pgtable.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/proc-fns.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/pgtable-prot.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/tlbflush.h \
    $(wildcard include/config/arm64/workaround/repeat/tlbi.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/fixmap.h \
    $(wildcard include/config/acpi/apei/ghes.h) \
    $(wildcard include/config/arm/sde/interface.h) \
    $(wildcard include/config/unmap/kernel/at/el0.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/boot.h \
  /home/shan/android-kernel/common/include/asm-generic/fixmap.h \
  /home/shan/android-kernel/common/include/asm-generic/pgtable_uffd.h \
    $(wildcard include/config/have/arch/userfaultfd/wp.h) \
  /home/shan/android-kernel/common/include/linux/page_pinner.h \
  /home/shan/android-kernel/common/include/linux/huge_mm.h \
  /home/shan/android-kernel/common/include/linux/sched/coredump.h \
    $(wildcard include/config/core/dump/default/elf/headers.h) \
  /home/shan/android-kernel/common/include/linux/vmstat.h \
    $(wildcard include/config/vm/event/counters.h) \
    $(wildcard include/config/debug/tlbflush.h) \
    $(wildcard include/config/debug/vm/vmacache.h) \
  /home/shan/android-kernel/common/include/linux/vm_event_item.h \
    $(wildcard include/config/memory/balloon.h) \
    $(wildcard include/config/balloon/compaction.h) \
  /home/shan/android-kernel/common/include/linux/sched/mm.h \
    $(wildcard include/config/arch/has/membarrier/callbacks.h) \
  /home/shan/android-kernel/common/include/linux/sync_core.h \
    $(wildcard include/config/arch/has/sync/core/before/usermode.h) \
  /home/shan/android-kernel/common/include/linux/input.h \
  /home/shan/android-kernel/common/include/uapi/linux/input.h \
  /home/shan/android-kernel/common/include/uapi/linux/input-event-codes.h \
  /home/shan/android-kernel/common/include/linux/device.h \
    $(wildcard include/config/debug/devres.h) \
    $(wildcard include/config/energy/model.h) \
    $(wildcard include/config/generic/msi/irq/domain.h) \
    $(wildcard include/config/pinctrl.h) \
    $(wildcard include/config/generic/msi/irq.h) \
    $(wildcard include/config/dma/ops.h) \
    $(wildcard include/config/dma/declare/coherent.h) \
    $(wildcard include/config/dma/cma.h) \
    $(wildcard include/config/arch/has/sync/dma/for/device.h) \
    $(wildcard include/config/arch/has/sync/dma/for/cpu.h) \
    $(wildcard include/config/arch/has/sync/dma/for/cpu/all.h) \
    $(wildcard include/config/dma/ops/bypass.h) \
    $(wildcard include/config/of.h) \
    $(wildcard include/config/devtmpfs.h) \
    $(wildcard include/config/sysfs/deprecated.h) \
  /home/shan/android-kernel/common/include/linux/dev_printk.h \
  /home/shan/android-kernel/common/include/linux/energy_model.h \
  /home/shan/android-kernel/common/include/linux/sched/cpufreq.h \
    $(wildcard include/config/cpu/freq.h) \
  /home/shan/android-kernel/common/include/linux/sched/topology.h \
    $(wildcard include/config/sched/debug.h) \
    $(wildcard include/config/sched/mc.h) \
  /home/shan/android-kernel/common/include/linux/sched/idle.h \
  /home/shan/android-kernel/common/include/linux/sched/sd_flags.h \
  /home/shan/android-kernel/common/include/linux/klist.h \
  /home/shan/android-kernel/common/include/linux/pm.h \
    $(wildcard include/config/vt/console/sleep.h) \
    $(wildcard include/config/pm.h) \
    $(wildcard include/config/pm/clk.h) \
    $(wildcard include/config/pm/generic/domains.h) \
  /home/shan/android-kernel/common/include/linux/device/bus.h \
    $(wildcard include/config/acpi.h) \
  /home/shan/android-kernel/common/include/linux/device/class.h \
  /home/shan/android-kernel/common/include/linux/device/driver.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/device.h \
  /home/shan/android-kernel/common/include/linux/pm_wakeup.h \
  /home/shan/android-kernel/common/include/linux/mod_devicetable.h \
  /home/shan/android-kernel/common/include/linux/input/mt.h \
  /home/shan/android-kernel/common/include/linux/kprobes.h \
    $(wildcard include/config/kretprobes.h) \
    $(wildcard include/config/kprobes/sanity/test.h) \
    $(wildcard include/config/optprobes.h) \
    $(wildcard include/config/kprobes/on/ftrace.h) \
  /home/shan/android-kernel/common/include/linux/ftrace.h \
    $(wildcard include/config/function/tracer.h) \
    $(wildcard include/config/dynamic/ftrace/with/regs.h) \
    $(wildcard include/config/dynamic/ftrace/with/direct/calls.h) \
    $(wildcard include/config/have/dynamic/ftrace/with/direct/calls.h) \
    $(wildcard include/config/stack/tracer.h) \
    $(wildcard include/config/frame/pointer.h) \
    $(wildcard include/config/function/profiler.h) \
    $(wildcard include/config/ftrace/syscalls.h) \
  /home/shan/android-kernel/common/include/linux/trace_clock.h \
  arch/arm64/include/generated/asm/trace_clock.h \
  /home/shan/android-kernel/common/include/asm-generic/trace_clock.h \
  /home/shan/android-kernel/common/include/linux/kallsyms.h \
    $(wildcard include/config/kallsyms/all.h) \
  /home/shan/android-kernel/common/include/linux/ptrace.h \
  /home/shan/android-kernel/common/include/linux/pid_namespace.h \
    $(wildcard include/config/pid/ns.h) \
  /home/shan/android-kernel/common/include/linux/nsproxy.h \
  /home/shan/android-kernel/common/include/linux/ns_common.h \
  /home/shan/android-kernel/common/include/uapi/linux/ptrace.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/ftrace.h \
  /home/shan/android-kernel/common/include/linux/compat.h \
    $(wildcard include/config/arch/has/syscall/wrapper.h) \
    $(wildcard include/config/x86/x32/abi.h) \
    $(wildcard include/config/compat/old/sigaction.h) \
    $(wildcard include/config/odd/rt/sigaction.h) \
  /home/shan/android-kernel/common/include/linux/socket.h \
  arch/arm64/include/generated/uapi/asm/socket.h \
  /home/shan/android-kernel/common/include/uapi/asm-generic/socket.h \
  arch/arm64/include/generated/uapi/asm/sockios.h \
  /home/shan/android-kernel/common/include/uapi/asm-generic/sockios.h \
  /home/shan/android-kernel/common/include/uapi/linux/sockios.h \
  /home/shan/android-kernel/common/include/linux/uio.h \
    $(wildcard include/config/arch/has/copy/mc.h) \
  /home/shan/android-kernel/common/include/uapi/linux/uio.h \
  /home/shan/android-kernel/common/include/uapi/linux/socket.h \
  /home/shan/android-kernel/common/include/uapi/linux/if.h \
  /home/shan/android-kernel/common/include/uapi/linux/libc-compat.h \
  /home/shan/android-kernel/common/include/uapi/linux/hdlc/ioctl.h \
  /home/shan/android-kernel/common/include/uapi/linux/aio_abi.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/syscall_wrapper.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/kprobes.h \
  /home/shan/android-kernel/common/include/asm-generic/kprobes.h \
  /home/shan/android-kernel/HOOK/include/teargame_compat.h \
  include/generated/uapi/linux/version.h \
  /home/shan/android-kernel/common/include/linux/highmem.h \
    $(wildcard include/config/x86/32.h) \
    $(wildcard include/config/debug/highmem.h) \
  /home/shan/android-kernel/common/include/linux/hardirq.h \
  /home/shan/android-kernel/common/include/linux/context_tracking_state.h \
    $(wildcard include/config/context/tracking.h) \
  /home/shan/android-kernel/common/include/linux/ftrace_irq.h \
    $(wildcard include/config/hwlat/tracer.h) \
  /home/shan/android-kernel/common/include/linux/vtime.h \
    $(wildcard include/config/virt/cpu/accounting.h) \
    $(wildcard include/config/irq/time/accounting.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/hardirq.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/irq.h \
  /home/shan/android-kernel/common/include/asm-generic/irq.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/kvm_arm.h \
  /home/shan/android-kernel/common/include/linux/irq_cpustat.h \
  /home/shan/android-kernel/common/arch/arm64/include/asm/cacheflush.h \
  /home/shan/android-kernel/common/include/linux/kgdb.h \
    $(wildcard include/config/have/arch/kgdb.h) \
    $(wildcard include/config/kgdb.h) \
    $(wildcard include/config/serial/kgdb/nmi.h) \
    $(wildcard include/config/kgdb/honour/blocklist.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/kgdb.h \
  /home/shan/android-kernel/common/include/asm-generic/cacheflush.h \
  arch/arm64/include/generated/asm/kmap_types.h \
  /home/shan/android-kernel/common/include/asm-generic/kmap_types.h \
  /home/shan/android-kernel/common/include/linux/io.h \
    $(wildcard include/config/has/ioport/map.h) \
    $(wildcard include/config/pci.h) \
  /home/shan/android-kernel/common/arch/arm64/include/asm/io.h \
  /home/shan/android-kernel/common/include/linux/log_mmiorw.h \
    $(wildcard include/config/trace/mmio/access.h) \
  arch/arm64/include/generated/asm/early_ioremap.h \
  /home/shan/android-kernel/common/include/asm-generic/early_ioremap.h \
    $(wildcard include/config/generic/early/ioremap.h) \
  /home/shan/android-kernel/common/include/asm-generic/io.h \
    $(wildcard include/config/generic/iomap.h) \
    $(wildcard include/config/generic/ioremap.h) \
    $(wildcard include/config/virt/to/bus.h) \
  /home/shan/android-kernel/common/include/asm-generic/pci_iomap.h \
    $(wildcard include/config/no/generic/pci/ioport/map.h) \
    $(wildcard include/config/generic/pci/iomap.h) \
  /home/shan/android-kernel/common/include/linux/logic_pio.h \
    $(wildcard include/config/indirect/pio.h) \
  /home/shan/android-kernel/common/include/linux/fwnode.h \
  /home/shan/android-kernel/HOOK/include/teargame_config.h \
    $(wildcard include/config/teargame/randomize.h) \
    $(wildcard include/config/teargame/cache/buckets.h) \
    $(wildcard include/config/teargame/touch/slots.h) \
    $(wildcard include/config/teargame/debug.h) \
    $(wildcard include/config/teargame/shadow/page.h) \
    $(wildcard include/config/teargame/syscall/block.h) \
  /home/shan/android-kernel/HOOK/include/teargame_cmd.h \
  /home/shan/android-kernel/HOOK/include/teargame_types.h \
  /home/shan/android-kernel/HOOK/include/teargame_security.h \
  /home/shan/android-kernel/common/include/linux/swap.h \
    $(wildcard include/config/frontswap.h) \
    $(wildcard include/config/memcg/swap.h) \
  /home/shan/android-kernel/common/include/linux/memcontrol.h \
  /home/shan/android-kernel/common/include/linux/cgroup.h \
    $(wildcard include/config/cgroup/cpuacct.h) \
    $(wildcard include/config/sock/cgroup/data.h) \
    $(wildcard include/config/cgroup/net/prio.h) \
    $(wildcard include/config/cgroup/net/classid.h) \
    $(wildcard include/config/cgroup/data.h) \
    $(wildcard include/config/cgroup/bpf.h) \
  /home/shan/android-kernel/common/include/uapi/linux/cgroupstats.h \
  /home/shan/android-kernel/common/include/uapi/linux/taskstats.h \
  /home/shan/android-kernel/common/include/linux/seq_file.h \
  /home/shan/android-kernel/common/include/linux/user_namespace.h \
    $(wildcard include/config/inotify/user.h) \
    $(wildcard include/config/persistent/keyrings.h) \
  /home/shan/android-kernel/common/include/linux/kernel_stat.h \
  /home/shan/android-kernel/common/include/linux/interrupt.h \
    $(wildcard include/config/irq/forced/threading.h) \
    $(wildcard include/config/generic/irq/probe.h) \
    $(wildcard include/config/irq/timings.h) \
  /home/shan/android-kernel/common/include/linux/irqreturn.h \
  /home/shan/android-kernel/common/include/linux/cgroup-defs.h \
  /home/shan/android-kernel/common/include/linux/u64_stats_sync.h \
  arch/arm64/include/generated/asm/local64.h \
  /home/shan/android-kernel/common/include/asm-generic/local64.h \
  arch/arm64/include/generated/asm/local.h \
  /home/shan/android-kernel/common/include/asm-generic/local.h \
  /home/shan/android-kernel/common/include/linux/bpf-cgroup.h \
  /home/shan/android-kernel/common/include/linux/bpf.h \
    $(wildcard include/config/bpf/jit/always/on.h) \
    $(wildcard include/config/bpf/stream/parser.h) \
    $(wildcard include/config/inet.h) \
  /home/shan/android-kernel/common/include/uapi/linux/bpf.h \
    $(wildcard include/config/efficient/unaligned/access.h) \
    $(wildcard include/config/ip/route/classid.h) \
    $(wildcard include/config/bpf/kprobe/override.h) \
    $(wildcard include/config/xfrm.h) \
    $(wildcard include/config/bpf/lirc/mode2.h) \
  /home/shan/android-kernel/common/include/uapi/linux/bpf_common.h \
  /home/shan/android-kernel/common/include/linux/file.h \
  /home/shan/android-kernel/common/include/linux/bpf_types.h \
    $(wildcard include/config/bpf/lsm.h) \
    $(wildcard include/config/xdp/sockets.h) \
  /home/shan/android-kernel/common/include/linux/psi_types.h \
  /home/shan/android-kernel/common/include/linux/kthread.h \
  /home/shan/android-kernel/common/include/linux/cgroup_subsys.h \
    $(wildcard include/config/cgroup/device.h) \
    $(wildcard include/config/cgroup/freezer.h) \
    $(wildcard include/config/cgroup/perf.h) \
    $(wildcard include/config/cgroup/hugetlb.h) \
    $(wildcard include/config/cgroup/pids.h) \
    $(wildcard include/config/cgroup/rdma.h) \
    $(wildcard include/config/cgroup/debug.h) \
  /home/shan/android-kernel/common/include/linux/page_counter.h \
  /home/shan/android-kernel/common/include/linux/vmpressure.h \
  /home/shan/android-kernel/common/include/linux/eventfd.h \
    $(wildcard include/config/eventfd.h) \
  /home/shan/android-kernel/common/include/linux/writeback.h \
  /home/shan/android-kernel/common/include/linux/flex_proportions.h \
  /home/shan/android-kernel/common/include/linux/backing-dev-defs.h \
    $(wildcard include/config/debug/fs.h) \
  /home/shan/android-kernel/common/include/linux/blk_types.h \
    $(wildcard include/config/blk/cgroup/iocost.h) \
    $(wildcard include/config/blk/inline/encryption.h) \
    $(wildcard include/config/dm/default/key.h) \
    $(wildcard include/config/blk/dev/integrity.h) \
  /home/shan/android-kernel/common/include/linux/bvec.h \
  /home/shan/android-kernel/common/include/linux/blk-cgroup.h \
  /home/shan/android-kernel/common/include/linux/blkdev.h \
    $(wildcard include/config/blk/rq/alloc/time.h) \
    $(wildcard include/config/blk/wbt.h) \
    $(wildcard include/config/blk/dev/zoned.h) \
    $(wildcard include/config/blk/dev/bsg.h) \
    $(wildcard include/config/blk/dev/throttling.h) \
    $(wildcard include/config/blk/debug/fs.h) \
  /home/shan/android-kernel/common/include/linux/sched/clock.h \
    $(wildcard include/config/have/unstable/sched/clock.h) \
  /home/shan/android-kernel/common/include/uapi/linux/major.h \
  /home/shan/android-kernel/common/include/linux/genhd.h \
    $(wildcard include/config/fail/make/request.h) \
    $(wildcard include/config/cdrom.h) \
  /home/shan/android-kernel/common/include/linux/pagemap.h \
  /home/shan/android-kernel/common/include/linux/hugetlb_inline.h \
  /home/shan/android-kernel/common/include/linux/sched/debug.h \
  /home/shan/android-kernel/common/include/linux/mempool.h \
  /home/shan/android-kernel/common/include/linux/bio.h \
  /home/shan/android-kernel/common/include/linux/bsg.h \
  /home/shan/android-kernel/common/include/uapi/linux/bsg.h \
  /home/shan/android-kernel/common/include/linux/scatterlist.h \
    $(wildcard include/config/need/sg/dma/length.h) \
    $(wildcard include/config/debug/sg.h) \
    $(wildcard include/config/sgl/alloc.h) \
    $(wildcard include/config/arch/no/sg/chain.h) \
    $(wildcard include/config/sg/pool.h) \
  /home/shan/android-kernel/common/include/uapi/linux/blkzoned.h \
  /home/shan/android-kernel/common/include/linux/elevator.h \
  /home/shan/android-kernel/common/include/linux/hashtable.h \
  /home/shan/android-kernel/common/include/linux/blk-mq.h \
    $(wildcard include/config/fail/io/timeout.h) \
  /home/shan/android-kernel/common/include/linux/sbitmap.h \
  /home/shan/android-kernel/common/include/linux/node.h \
    $(wildcard include/config/hmem/reporting.h) \
    $(wildcard include/config/memory/hotplug/sparse.h) \
  /home/shan/android-kernel/common/include/linux/swapops.h \

/home/shan/android-kernel/HOOK/src/memory_safe.o: $(deps_/home/shan/android-kernel/HOOK/src/memory_safe.o)

$(deps_/home/shan/android-kernel/HOOK/src/memory_safe.o):
