#!/system/bin/sh
# 一键安装脚本
# 使用方法: 
# 1. 将本脚本(install.sh)和 vfs_core.ko 放到手机同一目录 (如 /data/local/tmp)
# 2. chmod +x install.sh
# 3. su -c ./install.sh

MODID="vfs_core"
MODPATH="/data/adb/modules/$MODID"
KO_FILE="vfs_core.ko"

echo "========================================"
echo "      内核模块一键安装工具"
echo "========================================"

# 检查 Root 权限
if [ "$(id -u)" -ne 0 ]; then
    echo "[-] 错误：请使用 Root 权限运行此脚本！"
    echo "    命令: su -c ./install.sh"
    exit 1
fi

# 检查环境 (Magisk/KernelSU)
if [ ! -d "/data/adb/modules" ]; then
    echo "[-] 错误：未检测到 Magisk 或 KernelSU 模块目录 (/data/adb/modules)"
    echo "    请确认手机已 Root 并安装了 Magisk/KernelSU"
    exit 1
fi

# 1. 检查当前目录下是否有 ko 文件
if [ ! -f "./$KO_FILE" ]; then
    echo "[-] 错误：当前目录下未找到 $KO_FILE"
    echo "    请将 install.sh 和 $KO_FILE 放在同一文件夹内"
    exit 1
fi

# 2. 清理旧版本
if [ -d "$MODPATH" ]; then
    echo "[*] 检测到旧版本，正在清理..."
    rm -rf "$MODPATH"
fi

# 3. 创建 Magisk 模块目录
echo "[*] 创建模块目录: $MODPATH"
mkdir -p "$MODPATH"

# 4. 复制 ko 文件
echo "[*] 复制内核模块..."
cp "./$KO_FILE" "$MODPATH/"
chmod 644 "$MODPATH/$KO_FILE"

# 5. 创建 module.prop
echo "[*] 生成模块配置文件..."
cat > "$MODPATH/module.prop" <<EOF
id=$MODID
name=Core Services
version=v1.0
versionCode=1
author=Google
description=System Core Service Driver
EOF
chmod 644 "$MODPATH/module.prop"

# 6. 创建启动脚本 service.sh
echo "[*] 生成开机自启脚本..."
cat > "$MODPATH/service.sh" <<EOF
#!/system/bin/sh
MODDIR=\${0%/*}
sleep 5
LOG="\$MODDIR/last_load.log"
DMESG_LOG="\$MODDIR/last_dmesg.log"
date > "\$LOG"
echo "uname: \$(uname -a)" >> "\$LOG"
echo "cmdline: \$(cat /proc/cmdline 2>/dev/null)" >> "\$LOG"
echo "insmod: \$MODDIR/$KO_FILE" >> "\$LOG"
insmod "\$MODDIR/$KO_FILE" >> "\$LOG" 2>&1
RC=\$?
echo "rc=\$RC" >> "\$LOG"
if [ "\$RC" -ne 0 ]; then
  dmesg | tail -n 200 > "\$DMESG_LOG" 2>/dev/null
fi
exit "\$RC"
EOF
chmod 755 "$MODPATH/service.sh"

# 7. 标记模块为更新/启用状态
touch "$MODPATH/update"
rm -f "$MODPATH/disable"

# 8. 尝试立即加载
echo "[*] 正在尝试立即加载模块..."
INSOUT="$(insmod "$MODPATH/$KO_FILE" 2>&1)"
RC=$?
if [ "$RC" -eq 0 ]; then
    echo "[+] 立即加载成功！"
else
    # 检查是否已经加载
    if grep -q "$MODID" /proc/modules; then
        echo "[!] 模块已在运行中"
    else
        echo "[!] 立即加载失败"
        if [ -n "$INSOUT" ]; then
            echo "$INSOUT"
        fi
        echo "------ dmesg (tail 120) ------"
        dmesg | tail -n 120 2>/dev/null
    fi
fi

echo "========================================"
echo "[+] 安装完成！"
echo "[+] 模块已植入 /data/adb/modules/$MODID"
echo "[+] 下次重启后将自动加载"
echo "========================================"
