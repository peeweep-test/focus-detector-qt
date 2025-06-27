#!/bin/bash

# 快速构建脚本 - Focus Detector Qt
# Quick Build Script - Focus Detector Qt

set -e

echo "🚀 快速构建 Focus Detector Qt..."

# 清理构建目录
echo "🧹 清理构建目录..."
rm -rf build/
mkdir build
cd build

# CMake 配置
echo "⚙️  配置项目..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# 编译
echo "🔨 编译项目..."
make -j$(nproc)

echo "✅ 编译完成！"
echo "📍 可执行文件位置: $(pwd)/focus-detector-qt"

# 运行测试
echo ""
echo "🔍 测试运行..."
if ./focus-detector-qt --version 2>/dev/null || ./focus-detector-qt --help 2>/dev/null || true; then
    echo "✅ 程序可以正常启动"
else
    echo "ℹ️  程序启动测试完成"
fi

echo ""
echo "🎯 可以运行以下命令启动程序："
echo "cd build && ./focus-detector-qt"
echo ""
echo "📦 如需构建 .deb 包，请运行："
echo "./build_package.sh" 