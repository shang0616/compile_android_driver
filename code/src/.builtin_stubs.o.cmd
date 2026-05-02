cmd_/home/shan/android-kernel2/HOOK/src/builtin_stubs.o := clang -Wp,-MMD,/home/shan/android-kernel2/HOOK/src/.builtin_stubs.o.d -nostdinc -isystem /usr/lib/llvm-18/lib/clang/18/include -I/home/shan/android-kernel2/common/arch/arm64/include -I./arch/arm64/include/generated -I/home/shan/android-kernel2/common/include -I./include -I/home/shan/android-kernel2/common/arch/arm64/include/uapi -I./arch/arm64/include/generated/uapi -I/home/shan/android-kernel2/common/include/uapi -I./include/generated/uapi -include /home/shan/android-kernel2/common/include/linux/kconfig.h -D__KERNEL__ -mlittle-endian -DKASAN_SHADOW_SCALE_SHIFT=3 -Qunused-arguments -fmacro-prefix-map=/home/shan/android-kernel2/common/= -D__ASSEMBLY__ -fno-PIE --target=aarch64-linux-gnu --prefix=/usr/bin/aarch64-linux-gnu- --gcc-toolchain=/usr -fno-integrated-as -Werror=unknown-warning-option -fno-asynchronous-unwind-tables -fno-unwind-tables -DKASAN_SHADOW_SCALE_SHIFT=3 -Wa,-gdwarf-2  -DMODULE  -c -o /home/shan/android-kernel2/HOOK/src/builtin_stubs.o /home/shan/android-kernel2/HOOK/src/builtin_stubs.S

source_/home/shan/android-kernel2/HOOK/src/builtin_stubs.o := /home/shan/android-kernel2/HOOK/src/builtin_stubs.S

deps_/home/shan/android-kernel2/HOOK/src/builtin_stubs.o := \
  /home/shan/android-kernel2/common/include/linux/kconfig.h \
    $(wildcard include/config/cc/version/text.h) \
    $(wildcard include/config/cpu/big/endian.h) \
    $(wildcard include/config/booger.h) \
    $(wildcard include/config/foo.h) \

/home/shan/android-kernel2/HOOK/src/builtin_stubs.o: $(deps_/home/shan/android-kernel2/HOOK/src/builtin_stubs.o)

$(deps_/home/shan/android-kernel2/HOOK/src/builtin_stubs.o):
