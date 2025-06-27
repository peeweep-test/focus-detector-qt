# 焦点检测器 - Qt6 Widgets 项目

## 项目概述

这是一个基于 Qt6 Widgets 和 CMake 的焦点检测应用程序，用于监控和记录窗口焦点状态的变化。应用程序能够精确记录窗口获得和失去焦点的时间，并在界面和命令行中实时显示这些信息。

**🔄 v0.3 重大更新：从QML重构为Qt Widgets，解决窗口显示问题！**

## 功能特性

### 1. 焦点状态检测
- **实时监控**: 监控窗口焦点状态变化
- **视觉反馈**: 窗口获得焦点时背景变为浅蓝色，失去焦点时变为白色
- **状态指示器**: 显示当前窗口焦点状态（获得/失去焦点）

### 2. 精确时间记录
- **毫秒精度**: 记录焦点变化的精确时间（精确到毫秒）
- **时间格式**: yyyy-MM-dd hh:mm:ss.zzz
- **实时更新**: 焦点状态变化时自动更新时间显示
- **焦点切换时间差**: 计算并显示窗口获得焦点与全局最后失去焦点之间的时间差

### 3. 统一窗口设计 
- **所有窗口一致**: 第一个窗口和后续创建的窗口完全相同
- **智能标识**: 主窗口显示"(主窗口)"标识，其他窗口显示普通标识
- **统一功能**: 每个窗口都有"打开新窗口"按钮，可以无限创建窗口
- **智能关闭**: 主窗口显示"退出程序"，其他窗口显示"关闭窗口"

### 4. 多窗口支持
- **无限制创建**: 可以创建任意数量的窗口
- **独立监控**: 每个窗口独立监控自己的焦点状态
- **唯一标识**: 每个窗口都有唯一的ID标识
- **级联创建**: 任何窗口都可以创建新窗口

### 5. 日志系统
- **控制台输出**: 所有焦点变化事件都会在命令行中输出日志
- **全局记录**: 主类记录所有窗口的焦点变化历史
- **格式化日志**: 日志包含时间戳、窗口ID和操作类型

## 技术架构

### 主要组件

1. **FocusManager 类**
   - 负责焦点事件的记录和管理
   - 提供精确时间获取功能
   - 输出日志到控制台
   - 发送焦点变化信号

2. **FocusWindow 类** (Qt Widget)
   - **统一窗口组件** - 所有窗口都使用这个类
   - 继承自QWidget，稳定可靠
   - 支持主窗口和普通窗口两种模式
   - 包含完整的焦点检测功能和UI界面

3. **main.cpp** (应用程序入口)
   - 使用QApplication代替QGuiApplication
   - 创建第一个窗口（主窗口）
   - 简洁的应用程序初始化

### Qt Widgets的优势
✅ **稳定可靠**: 成熟的GUI框架，兼容性好  
✅ **窗口管理**: 原生窗口支持，无显示问题  
✅ **事件处理**: 完善的焦点事件处理机制  
✅ **跨平台**: 更好的跨平台兼容性  
✅ **易于调试**: 传统的调试和开发流程  

### 技术栈
- **Qt6 Widgets**: 传统Qt GUI框架
- **CMake**: 构建系统
- **C++**: 纯C++实现，无QML依赖

## 使用说明

### 1. 快速开始
```bash
# 方式1: 使用Makefile (推荐)
make build          # 构建项目
make package        # 创建.deb安装包

# 方式2: 使用脚本
./quick_build.sh    # 快速构建
./build_package.sh  # 构建debian包
./cpack_build.sh    # 使用CPack构建包
```

### 2. 手动编译
```bash
# 创建构建目录
mkdir build
cd build

# 配置项目
cmake .. -DCMAKE_BUILD_TYPE=Release

# 编译项目
make -j$(nproc)
```

### 3. 运行应用程序
```bash
# 从构建目录运行
cd build && ./focus-detector-qt

# 或使用演示脚本
./run_demo.sh
```

### 4. 安装和打包

#### 安装到系统
```bash
make install               # 安装到系统
sudo dpkg -i ../focus-detector-qt_*.deb  # 安装deb包
```

#### 创建安装包
```bash
make package              # 创建debian包 (推荐)
make package-cpack        # 使用CPack创建包
```

#### 清理构建文件
```bash
make clean               # 清理所有构建文件
```

### 3. 操作指南

#### 基本操作
1. **启动应用**: 运行后会自动显示主窗口（标题显示"主窗口"）
2. **观察焦点变化**: 
   - 点击窗口或切换到其他应用来测试焦点变化
   - 观察背景颜色变化（浅蓝色=有焦点，白色=无焦点）
3. **查看时间记录**: 在文本框中查看精确的焦点获得/失去时间

#### 多窗口测试
1. **创建新窗口**: 点击任意窗口中的"🪟 打开新窗口"按钮
2. **观察窗口区别**: 
   - 主窗口：显示"窗口-1 (主窗口)"，按钮显示"退出程序"
   - 其他窗口：显示"窗口-xxxx"，按钮显示"关闭窗口"
3. **窗口切换**: 在多个窗口间切换来测试焦点变化
4. **独立监控**: 每个窗口都会独立记录自己的焦点状态
5. **级联创建**: 从任何窗口都可以创建更多新窗口

#### 日志查看
- 查看命令行输出，所有焦点变化都会有详细的日志记录
- 日志格式: `[时间戳] 窗口 窗口ID 操作类型`

## 界面说明

### 主界面元素

1. **窗口标识区域**: 
   - 主窗口：绿色背景，显示"窗口ID: 窗口-1 (主窗口)"
   - 其他窗口：橙色背景，显示"窗口ID: 窗口-xxxx"

2. **状态指示器**: 
   - 绿色 + "✓ 窗口已获得焦点" = 窗口有焦点
   - 橙色 + "⚠ 窗口失去焦点" = 窗口无焦点

3. **时间记录区域**:
   - 获得焦点时间文本框
   - 失去焦点时间文本框
   - 焦点切换耗时文本框（显示时间差，根据时长用不同颜色标识）

4. **控制按钮区域**: 
   - "🪟 打开新窗口"按钮 - 所有窗口都有
   - 主窗口："❌ 退出程序" - 关闭整个应用程序
   - 其他窗口："❌ 关闭窗口" - 只关闭当前窗口

### 窗口特性
- **统一尺寸**: 所有窗口都是 450x420 像素
- **背景颜色**: 根据焦点状态自动变化
- **文本框**: 只读模式，支持文本选择和复制
- **现代样式**: 使用CSS样式美化界面

## 开发说明

### 核心类介绍

#### FocusManager
```cpp
class FocusManager : public QObject {
    Q_OBJECT
public:
    // 记录焦点获得/失去
    void recordFocusGained(const QString &windowId);
    void recordFocusLost(const QString &windowId);
    
    // 获取当前时间字符串
    QString getCurrentTimeString();
    
signals:
    // 焦点变化信号
    void focusChanged(const QString &windowId, bool hasFocus, const QString &timestamp);
};
```

#### FocusWindow (Qt Widget)
```cpp
class FocusWindow : public QWidget {
    Q_OBJECT
public:
    FocusWindow(QWidget *parent = nullptr, 
                const QString &windowId = "新窗口",
                bool isMainWindow = false,
                FocusManager *globalManager = nullptr);

protected:
    // 焦点事件处理
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void changeEvent(QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
};
```

### 焦点检测机制
- **focusInEvent/focusOutEvent**: 处理控件焦点事件
- **changeEvent(ActivationChange)**: 处理窗口激活状态变化
- **双重检测**: 确保焦点变化被准确捕获

## 🗂️ 打包说明

### 打包方式对比

| 方式 | 命令 | 输出 | 优势 | 适用场景 |
|------|------|------|------|----------|
| **Debian 打包** | `make package` | `.deb` 文件 | 标准Linux包管理 | 生产环境部署 |
| **CPack 打包** | `make package-cpack` | `.deb` + `.tar.gz` | 跨平台支持 | 多平台分发 |
| **源码包** | `make build` | 可执行文件 | 快速测试 | 开发调试 |

### 详细打包步骤

#### 1. 环境准备
```bash
# 安装构建依赖
sudo apt update
sudo apt install build-essential debhelper cmake qt6-base-dev qt6-base-dev-tools libqt6widgets6

# 检查依赖
./build_package.sh  # 会自动检查并提示缺少的依赖
```

#### 2. Debian包构建
```bash
# 方式1: 使用Makefile (推荐)
make clean          # 清理之前的构建
make package        # 构建debian包

# 方式2: 直接使用脚本
./build_package.sh  # 全功能脚本，包含依赖检查

# 输出文件位置
ls -la ../focus-detector-qt_*.deb
```

#### 3. CPack包构建
```bash
# 构建多种格式的包
make package-cpack

# 生成的文件
ls -la build/focus-detector-qt-*.deb     # Debian包
ls -la build/focus-detector-qt-*.tar.gz  # 压缩包
```

#### 4. 包安装测试
```bash
# 安装deb包
sudo dpkg -i ../focus-detector-qt_*.deb

# 修复依赖问题（如果有）
sudo apt --fix-broken install

# 卸载包
sudo apt remove focus-detector-qt
```

### 包内容验证

#### 查看包信息
```bash
# 查看包详细信息
dpkg-deb --info ../focus-detector-qt_*.deb

# 查看包内文件列表
dpkg-deb --contents ../focus-detector-qt_*.deb

# 检查包依赖
dpkg-deb --field ../focus-detector-qt_*.deb Depends
```

#### 预期包内容
```
/usr/bin/focus-detector-qt          # 主程序
/usr/bin/run_demo.sh                # 演示脚本
/usr/share/doc/focus-detector-qt/   # 文档目录
├── README.md                       # 使用说明
├── DEMO.md                         # 演示文档
└── copyright                       # 版权信息
```

### 自动化构建

#### 使用Makefile
```bash
make help           # 查看所有可用命令
make build          # 快速构建
make test           # 测试运行
make package        # 创建安装包
make clean          # 清理构建文件
make install        # 安装到系统
```

#### CI/CD 集成
```bash
# 在持续集成环境中
./build_package.sh          # 检查依赖并构建
echo $?                     # 检查构建状态
ls -la ../focus-detector-qt_*.deb  # 验证输出
```

### 常见问题

**Q: 构建时提示缺少Qt6依赖？**
```bash
# Ubuntu/Debian
sudo apt install qt6-base-dev qt6-base-dev-tools libqt6widgets6

# 检查Qt6安装
qmake6 -query QT_VERSION
```

**Q: debian包构建失败？**
```bash
# 清理环境重试
make clean
rm -rf debian/.debhelper/
./build_package.sh
```

**Q: 包安装后找不到程序？**
```bash
# 检查安装路径
which focus-detector-qt
dpkg -L focus-detector-qt | grep bin
```

### 版本管理

当前版本信息在以下文件中定义：
- `CMakeLists.txt`: `VERSION 1.0.0`
- `debian/changelog`: 包版本历史
- 构建脚本会自动从CMakeLists.txt读取版本号

### 完整的打包工作流

```bash
# 1. 清理环境
make clean

# 2. 构建项目
make build

# 3. 测试运行
make test

# 4. 创建包
make package

# 5. 测试包
make test-package

# 6. 安装测试
sudo dpkg -i ../focus-detector-qt_*.deb
focus-detector-qt  # 运行程序

# 7. 清理（可选）
sudo apt remove focus-detector-qt
```

### 脚本功能总览

| 脚本 | 功能 | 使用场景 |
|------|------|----------|
| `quick_build.sh` | 快速构建和测试 | 开发调试 |
| `build_package.sh` | 完整的debian包构建 | 生产发布 |
| `cpack_build.sh` | 使用CPack多格式打包 | 跨平台分发 |
| `test_package.sh` | 验证生成的包 | 质量保证 |
| `run_demo.sh` | 运行演示 | 用户体验 |
| `Makefile` | 统一构建接口 | 自动化流程 |

## 扩展建议

1. **数据持久化**: 可以添加数据库支持，将焦点记录保存到文件
2. **统计功能**: 添加焦点时间统计分析功能
3. **配置选项**: 添加时间格式、日志级别等配置选项
4. **网络功能**: 支持将焦点数据发送到远程服务器
5. **主题支持**: 添加多种UI主题选择
6. **窗口管理**: 添加窗口列表，方便管理多个窗口

## 故障排除

### 常见问题

1. **编译错误**: 确保安装了Qt6开发环境和CMake
2. **焦点检测不准确**: 确认窗口管理器支持焦点事件
3. **时间显示异常**: 检查系统时间设置
4. **窗口不显示**: 已解决！使用Qt Widgets重构

### 依赖要求
- Qt6 Widgets 模块
- CMake 3.16 or 更高版本
- 支持C++标准的编译器

## 更新日志

### v0.3 (当前版本) - Qt Widgets重构 🔄
- 🔄 **架构重构**: 从QML完全重构为Qt Widgets
- ✅ **解决显示问题**: 窗口显示稳定可靠
- 🆕 **原生界面**: 使用Qt Widgets原生控件
- 🚀 **性能优化**: 更好的性能和响应速度
- 🔧 **代码简化**: 移除QML依赖，纯C++实现
- ✅ **保持功能**: 所有原有功能完全保留

### v0.2
- 统一窗口设计：所有窗口使用同一个组件
- 智能窗口标识：主窗口和普通窗口自动区分
- 优化界面布局：增大窗口尺寸，确保按钮正确显示
- 级联窗口创建：任何窗口都可以创建新窗口
- 智能关闭功能：主窗口退出程序，其他窗口只关闭自己

### v0.1 
- 基础焦点检测功能
- 精确时间记录
- 多窗口支持
- 控制台日志输出
- 响应式UI设计

---

**注意**: 项目已完全重构为Qt Widgets版本，解决了之前QML版本的窗口显示问题。新版本更加稳定可靠，适合生产环境使用。 