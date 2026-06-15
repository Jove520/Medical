# 3D Slicer 完全复现计划书 - 总论

## 1. 项目性质

本项目是 3D Slicer，一个跨平台医学图像可视化、分析、处理环境。它不是单纯的桌面 GUI 项目，而是一个大型科研和医学图像平台，核心由 C++、Qt、VTK、ITK、MRML、PythonQt、CLI 插件、脚本模块和扩展系统共同组成。

因此，“完全复现”不能只理解为把界面跑起来。真正的复现目标应包括：

1. 可重复构建同一版本源码。
2. 可生成 Windows、Linux、macOS 对应运行产物。
3. 可启动主程序并加载核心模块。
4. 可复现 MRML 场景、数据 IO、3D/2D 视图、DICOM、分割、标注、模型、体渲染等核心能力。
5. 可运行 Python API、CLI 模块、C++ Loadable 模块、Scripted 模块。
6. 可执行测试、打包和扩展构建流程。
7. 可说明依赖版本、构建参数、产物结构和验收标准。

## 2. 源码基线

当前本地源码位于：

```text
D:\AAAJove\3dslicer\Slicer
```

源码基线信息：

- 远程仓库：`https://github.com/Slicer/Slicer.git`
- 当前分支：`main`
- 当前 HEAD：`923abab79b1b6410b67556e127c39e8328e63ba4`
- HEAD 提交时间：`2026-06-13`
- 项目版本：`5.11.0`
- 应用名：`Slicer`
- 显示名：`3D Slicer`

源码规模约 4,780 个文件，主体集中在：

- `Modules`：核心业务模块。
- `Libs`：MRML、VTK/ITK 辅助库、分割核心等。
- `Base`：应用框架、Qt Core/GUI、Python API、CLI 基础设施。
- `CMake` 和 `SuperBuild`：构建系统和依赖编排。
- `Applications/SlicerApp`：主应用入口、窗口、图标、启动资源。

## 3. 技术主线

复现工程应围绕五条主线展开：

1. 构建主线：CMake、SuperBuild、ExternalProject。
2. 运行时主线：`SlicerApp`、`qSlicerApplication`、应用初始化、Launcher 环境。
3. 数据主线：MRML Scene、Node、Storage、Display、Logic。
4. 模块主线：Core、Loadable、Scripted、CLI 四类模块发现和加载。
5. 发布主线：CTest、CDash、CPack、NSIS/TGZ/DragNDrop、扩展 ABI 兼容。

## 4. 构建复现判断

源码强制要求 CMake policy 范围为 `3.28.0...3.28.0`，C++ 标准默认 `17`，也允许 `20`。Python 固定为 `3.12.10`。Qt 默认走 Qt5 最低 `5.15`，如果使用 Qt6 则最低 `6.8`。VTK 默认主版本为 `9`、小版本为 `9.6`，SuperBuild 中固定到 Slicer fork 的 VTK commit。

这意味着复现计划必须先冻结：

- 源码 commit。
- CMake 版本。
- 编译器和工具链。
- Qt 版本。
- Python 版本。
- SuperBuild 外部依赖 commit/hash。
- CMake cache 参数。
- Debug/Release 构建模式。

Windows 官方文档提示 Release 构建约需 15GB，Debug 约 60GB，构建时间可能 3 到 12 小时，所以复现工程必须预留磁盘、缓存和失败重试机制。

## 5. 架构复现判断

启动链路是：

```text
Applications/SlicerApp/Main.cxx
  -> qSlicerApplication
  -> qSlicerApplicationHelper::postInitializeApplication()
  -> 注册模块工厂
  -> 注册模块
  -> 实例化模块
  -> 创建主窗口
  -> 加载模块
  -> 处理命令行参数
```

模块体系分三大内置类型：

- Loadable：C++/Qt 动态库模块。
- Scripted：Python 模块。
- CLI：命令行算法模块，依赖 SlicerExecutionModel。

除此之外还有 Remote Modules，例如 `vtkAddon`、`MultiVolumeExplorer`、`SimpleFilters`、`BRAINSTools`、`CompareVolumes`、`LandmarkRegistration`、`SurfaceToolbox`，这些也必须纳入复现范围。

## 6. 复现目标分级

建议把目标分为三档：

### L1：源码可构建复现

能在指定平台完成 SuperBuild，生成 `Slicer.exe` 或对应平台应用。

### L2：功能可运行复现

能启动主界面、加载模块、打开医学数据、执行 CLI、运行 Python 脚本、保存和恢复 MRML 场景。

### L3：发布级完全复现

能通过核心测试，生成安装包，验证扩展构建，说明 ABI 兼容、依赖清单、许可证和安全边界。

## 7. 总体实施路线

后续计划书建议按这些章节展开：

1. 项目范围与复现标准。
2. 源码目录与架构拆解。
3. 构建环境与依赖冻结。
4. SuperBuild 依赖复现。
5. 应用启动与运行时框架。
6. MRML 数据模型与场景系统。
7. 模块系统复现。
8. 核心业务模块复现。
9. Python API 与脚本生态。
10. 测试体系与验收标准。
11. 打包、发布与扩展兼容。
12. 风险、成本、人员分工与里程碑。

本计划书后续章节应以当前源码基线为准，逐章补充实现路径、依赖清单、构建命令、验收用例和风险控制。
