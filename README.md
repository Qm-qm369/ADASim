# ADASim

## Automatic Driving Algorithm Simulator

基于 **C++17 / Qt / Linux** 开发的自动驾驶算法仿真平台。

ADASim 用于学习和验证自动驾驶基础算法，目前实现了从传感器数据生成、障碍物检测、路径规划、车辆控制到 Qt 可视化显示的基础闭环仿真流程。

当前版本：**V2.2**

---

## 项目特点

* 基于 Qt Widgets 实现二维自动驾驶仿真界面
* 使用 QThread 与 Qt Signal/Slot 实现后台数据处理和跨线程通信
* 支持 C++ 仿真程序与 Python Planner 通过 TCP/JSON 通信
* 支持障碍物检测、轨迹规划与车辆控制闭环
* 支持双误差控制与 Pure Pursuit 两种横向控制方式
* 支持纵向速度控制、TTC 计算和简化紧急制动
* 支持最近仿真帧的内存记录和时间轴回放
* 支持 INI 配置文件读取与保存
* 支持命令行配置文件参数
* 使用 Linux syslog 记录运行日志
* 使用 sigaction 处理 SIGINT / SIGTERM，并执行统一退出流程

---

# 技术栈

## 开发语言

* C++17
* Python 3

## GUI

* Qt Widgets
* Qt Signal/Slot

## 多线程

* QThread
* QObject::moveToThread
* Queued Connection
* BlockingQueuedConnection

## 构建系统

* CMake 3.16+
* Debug / Release 独立构建
* CMake install

## 网络通信

* TCP/IP
* QTcpServer
* QTcpSocket
* JSON
* 换行符消息分帧

## Linux

* syslog
* sigaction
* SIGINT
* SIGTERM
* ELF 可执行程序
* Linux 动态库依赖检查

## 配置管理

* QSettings
* INI 配置文件
* QCommandLineParser

---

# 当前项目目录

```text
ADASim/
├── CMakeLists.txt
├── README.md
├── cmake/
│   └── Version.h.in
├── config/
│   └── adasim.ini
├── src/
│   ├── algorithm/
│   │   ├── LongitudinalController
│   │   ├── ObstacleDetector
│   │   ├── PathPredictor
│   │   ├── PurePursuitController
│   │   ├── TrajectoryController
│   │   └── VehicleModel
│   ├── backend/
│   │   ├── DataLoader
│   │   ├── DataManager
│   │   └── SimulationRecorder
│   ├── communication/
│   │   └── Socket
│   ├── config/
│   │   └── ConfigManager
│   ├── gui/
│   │   ├── MainWindow
│   │   ├── View2D
│   │   ├── SensorView
│   │   └── ControlMonitor
│   ├── system/
│   │   ├── LinuxLogger
│   │   └── LinuxSignalHandler
│   └── main.cpp
└── tools/
    └── python_planner.py
```

---

# 当前运行架构

```text
                QThread
                   │
          ┌────────┴────────┐
          │                 │
     DataLoader        DataManager
          │                 │
          │ simulationTick  │
          ▼                 │
      MainWindow            │
          │                 │
          ├─ VehicleModel   │
          ├─ 横向控制器      │
          ├─ 纵向控制器      │
          └─ Recorder       │
          │                 │
          │ simulationFrame │
          └────────────────►│
                            │
                     ObstacleDetector
                            │
                            ▼
                       SocketServer
                            │
                         TCP/JSON
                            │
                            ▼
                     Python Planner
```

当前版本中主仿真控制流程仍位于 `MainWindow`，后续版本将进一步进行 GUI 与仿真核心逻辑解耦。

---

# C++ 与 Python Planner 通信

ADASim 作为 TCP Server，Python Planner 作为 TCP Client。

默认监听：

```text
127.0.0.1:8080
```

C++ 向 Python 发送障碍物消息：

```json
{
    "type": "OBSTACLES",
    "data": [
        {
            "local_x": 20.0,
            "local_y": 0.0,
            "dist": 20.0
        }
    ]
}
```

Python Planner 返回：

```json
{
    "type": "CONTROL",
    "steer_offset": 1.75
}
```

当前通信协议规定每条 JSON 消息以换行符结束。

---

# 配置文件

默认配置文件：

```text
config/adasim.ini
```

当前配置包含：

```ini
[controller]
heading_k=1
lateral_k=1.5
look_ahead=5
mode=DualError

[network]
planner_port=8080

[simulation]
planning_distance=20
target_speed=8
```

程序也支持通过命令行指定配置文件：

```bash
./ADASim --config /path/to/adasim.ini
```

---

# Debug 构建

```bash
cmake -S . \
      -B build-debug \
      -DCMAKE_BUILD_TYPE=Debug

cmake --build build-debug -j$(nproc)
```

运行：

```bash
./build-debug/ADASim
```

查看版本：

```bash
./build-debug/ADASim --version
```

查看帮助：

```bash
./build-debug/ADASim --help
```

---

# Release 构建

```bash
cmake -S . \
      -B build-release \
      -DCMAKE_BUILD_TYPE=Release

cmake --build build-release -j$(nproc)
```

---

# 运行 Python Planner

首先启动 ADASim：

```bash
./build-debug/ADASim
```

然后启动：

```bash
python3 tools/python_planner.py
```

默认情况下，ADASim 与 Python Planner 都使用 TCP 端口 `8080`。

如果修改 `adasim.ini` 中的 Planner 端口，需要保证 Python Planner 使用的端口与 ADASim 一致。

---

# 安装测试

可以在不修改系统目录的情况下测试 CMake install：

```bash
cmake --install build-release \
      --prefix "$PWD/install"
```

安装后结构：

```text
install/
└── bin/
    ├── ADASim
    └── config/
        └── adasim.ini
```

运行：

```bash
./install/bin/ADASim
```

---

# Linux 调试与检查

查看 ELF 文件信息：

```bash
file build-debug/ADASim
```

查看动态库依赖：

```bash
ldd build-debug/ADASim
```

查看 ELF Header：

```bash
readelf -h build-debug/ADASim
```

---

# 当前主要模块

### DataLoader

使用 QTimer 以固定周期产生仿真 Tick，并运行在后台 QThread 中。

### DataManager

负责车辆状态、点云数据、障碍物检测结果以及 Planner 消息之间的数据流转。

### ObstacleDetector

根据点云数据检测障碍物。

### VehicleModel

实现简化车辆运动模型。

### TrajectoryController

根据横向误差和航向误差计算车辆转向控制量。

### PurePursuitController

实现 Pure Pursuit 横向控制。

### LongitudinalController

负责目标速度、前方障碍物距离、TTC 和简化紧急制动控制。

### SimulationRecorder

在内存中保存最近的仿真帧，用于 GUI 时间轴回放。

### SocketServer

基于 QTcpServer / QTcpSocket 实现 ADASim 与 Python Planner 的 TCP 通信。

### ConfigManager

通过 QSettings 读取和保存 INI 参数。

### LinuxLogger

在 Linux 环境下使用 syslog 输出程序运行日志。

### LinuxSignalHandler

使用 sigaction 接收 SIGINT / SIGTERM，并通过 Qt 事件循环触发安全退出流程。

---

# 后续计划

V2.3 将重点进行仿真核心逻辑重构，把当前位于 MainWindow 中的车辆模型、控制、仿真状态推进等逻辑逐步拆分到独立 SimulationEngine 中，为后续 Headless 模式和 Linux 后台运行打基础。
