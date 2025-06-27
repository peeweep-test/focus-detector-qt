#!/bin/bash

# Focus Detector Qt - 包测试脚本
# Package Test Script

set -e

PACKAGE_NAME="focus-detector-qt"
DEB_FILE="../${PACKAGE_NAME}_*.deb"

echo "🧪 开始测试 Focus Detector Qt 包..."

# 检查包文件是否存在
echo "📦 检查包文件..."
if ! ls $DEB_FILE 1>/dev/null 2>&1; then
    echo "❌ 未找到 .deb 包文件"
    echo "请先运行 'make package' 构建包"
    exit 1
fi

DEB_FILE=$(ls ../focus-detector-qt_*.deb | head -1)
echo "✅ 找到包文件: $(basename $DEB_FILE)"

# 验证包结构
echo ""
echo "🔍 验证包结构..."
echo "📊 包信息:"
dpkg-deb --info "$DEB_FILE" | head -20

echo ""
echo "📂 包内容:"
dpkg-deb --contents "$DEB_FILE"

echo ""
echo "🔗 包依赖:"
dpkg-deb --field "$DEB_FILE" Depends

# 测试安装（模拟）
echo ""
echo "🔧 模拟安装测试..."
TEMP_DIR=$(mktemp -d)
dpkg-deb --extract "$DEB_FILE" "$TEMP_DIR"

echo "📁 提取的文件:"
find "$TEMP_DIR" -type f | sort

# 检查关键文件
EXPECTED_FILES=(
    "usr/bin/focus-detector-qt"
    "usr/bin/run_demo.sh"
    "usr/share/doc/focus-detector-qt/README.md"
)

echo ""
echo "✅ 检查关键文件:"
for file in "${EXPECTED_FILES[@]}"; do
    if [ -f "$TEMP_DIR/$file" ]; then
        echo "  ✓ $file"
    else
        echo "  ❌ $file (缺失)"
    fi
done

# 检查可执行文件权限
if [ -f "$TEMP_DIR/usr/bin/focus-detector-qt" ]; then
    if [ -x "$TEMP_DIR/usr/bin/focus-detector-qt" ]; then
        echo "  ✓ focus-detector-qt 有执行权限"
    else
        echo "  ❌ focus-detector-qt 缺少执行权限"
    fi
fi

# 清理临时目录
rm -rf "$TEMP_DIR"

echo ""
echo "📋 包大小信息:"
ls -lh "$DEB_FILE"

echo ""
echo "🎯 安装命令:"
echo "sudo dpkg -i $DEB_FILE"
echo "sudo apt --fix-broken install  # 如果有依赖问题"
echo ""
echo "📝 卸载命令:"
echo "sudo apt remove $PACKAGE_NAME"

echo ""
echo "✅ 包测试完成！" 