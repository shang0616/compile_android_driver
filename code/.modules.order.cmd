cmd_/home/shan/android-kernel/HOOK/modules.order := {   echo /home/shan/android-kernel/HOOK/vfs_core.ko; :; } | awk '!x[$$0]++' - > /home/shan/android-kernel/HOOK/modules.order
