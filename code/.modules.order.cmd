cmd_/home/shan/android-kernel2/HOOK/modules.order := {   echo /home/shan/android-kernel2/HOOK/vfs_core.ko; :; } | awk '!x[$$0]++' - > /home/shan/android-kernel2/HOOK/modules.order
