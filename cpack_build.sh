#!/bin/bash

# CPack构建脚本 - Focus Detector Qt
# CPack Build Script - Focus Detector Qt

set -e

echo "📦 使用 CPack 构建包..."

# 清理和创建构建目录
echo "🧹 准备构建环境..."
rm -rf build/
mkdir build
cd build

# CMake 配置
echo "⚙️  配置项目 (Release模式)..."
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr

# 编译
echo "🔨 编译项目..."
make -j$(nproc)

# 使用 CPack 打包
echo "📦 使用 CPack 生成包..."

# 生成 DEB 包
echo "🐧 生成 DEB 包..."
cpack -G DEB

# 生成 TGZ 包
echo "📂 生成 TGZ 包..."
cpack -G TGZ

echo ""
echo "🎉 CPack 打包完成！"
echo ""
echo "📋 生成的包文件："
ls -la focus-detector-qt-*.deb 2>/dev/null || echo "❌ 未找到 DEB 包"
ls -la focus-detector-qt-*.tar.gz 2>/dev/null || echo "❌ 未找到 TGZ 包"

# 显示 DEB 包信息
if ls focus-detector-qt-*.deb 1>/dev/null 2>&1; then
    DEB_FILE=$(ls focus-detector-qt-*.deb | head -1)
    echo ""
    echo "📊 DEB 包信息："
    dpkg-deb --info "$DEB_FILE"
    echo ""
    echo "✅ 安装命令："
    echo "sudo dpkg -i $DEB_FILE"
    echo "sudo apt --fix-broken install  # 如果有依赖问题"
fi

echo ""
echo "🏁 CPack 构建完成！" 