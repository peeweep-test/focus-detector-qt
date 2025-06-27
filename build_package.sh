#!/bin/bash

# Focus Detector Qt - Debian Package Build Script
# 焦点检测器 - Debian包构建脚本

set -e

echo "🚀 开始构建 Focus Detector Qt Debian 包..."

# 安装构建依赖
echo "📋 安装构建依赖..."
echo "运行命令: apt build-dep ."
sudo apt build-dep .

echo "✅ 构建依赖安装完成"

# 清理之前的构建
echo "🧹 清理之前的构建文件..."
rm -rf build/
rm -rf debian/.debhelper/
rm -f debian/debhelper-build-stamp
rm -f debian/files
rm -f debian/*.substvars
rm -f debian/*.debhelper.log
rm -f ../focus-detector-qt_*.deb
rm -f ../focus-detector-qt_*.changes
rm -f ../focus-detector-qt_*.buildinfo
rm -f ../focus-detector-qt_*.dsc
rm -f ../focus-detector-qt_*.tar.xz

# 检查代码
echo "🔍 检查代码完整性..."
if [ ! -f "main.cpp" ] || [ ! -f "focusmanager.cpp" ] || [ ! -f "focuswindow.cpp" ]; then
    echo "❌ 缺少核心源代码文件"
    exit 1
fi

if [ ! -f "CMakeLists.txt" ]; then
    echo "❌ 缺少 CMakeLists.txt 文件"
    exit 1
fi

echo "✅ 代码文件检查通过"

# 更新版本信息
echo "📝 更新版本信息..."
VERSION=$(grep "VERSION" CMakeLists.txt | head -1 | sed 's/.*VERSION \([0-9.]*\).*/\1/')
echo "当前版本: $VERSION"

# 构建源码包
echo "📦 开始构建 Debian 包..."
echo "构建命令: dpkg-buildpackage -us -uc -b"

# 使用 dpkg-buildpackage 构建包
if dpkg-buildpackage -us -uc -b; then
    echo ""
    echo "🎉 Debian 包构建成功！"
    echo ""
    echo "📋 构建结果："
    ls -la ../focus-detector-qt_*.deb 2>/dev/null || echo "❌ 未找到 .deb 文件"
    ls -la ../focus-detector-qt_*.changes 2>/dev/null || echo "ℹ️  未找到 .changes 文件"
    
    # 显示包信息
    if ls ../focus-detector-qt_*.deb 1>/dev/null 2>&1; then
        DEB_FILE=$(ls ../focus-detector-qt_*.deb | head -1)
        echo ""
        echo "📊 包信息："
        dpkg-deb --info "$DEB_FILE"
        echo ""
        echo "📂 包内容："
        dpkg-deb --contents "$DEB_FILE"
        echo ""
        echo "✅ 可以使用以下命令安装："
        echo "sudo dpkg -i $DEB_FILE"
        echo "如果有依赖问题，请运行："
        echo "sudo apt --fix-broken install"
    fi
else
    echo ""
    echo "❌ Debian 包构建失败！"
    echo "请检查上面的错误信息并修复问题"
    exit 1
fi

echo ""
echo "🏁 构建脚本执行完成！"
