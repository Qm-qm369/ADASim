# ADASim

## Automatic Driving Algorithm Simulator

基于 **C++17 / Qt / Linux** 开发的自动驾驶算法仿真平台。

ADASim 用于学习和验证自动驾驶基础算法，实现从传感器数据生成、障碍物检测、路径规划、车辆控制到 Qt 可视化的基础闭环仿真。仿真核心已从界面中拆出，支持图形界面和无界面两种运行方式。

当前版本：**v2.8.0**

许可证：MIT

---

## 项目特点

- 基于 Qt Widgets 的二维仿真界面
- 使用 QThread 与 Qt Signal/Slot 处理后台数据和跨线程通信
- C++ 仿真程序与 Python Planner 通过 TCP/JSON 通信（换行分帧）
- 障碍物检测、轨迹规划与车辆控制闭环
- Dual Error 与 Pure Pursuit 两种横向控制
- 纵向速度控制、TTC 计算和简化紧急制动
- 仿真帧记录与回放
- INI 配置文件，可用命令行指定路径
- Linux syslog 运行日志
- sigaction 处理 SIGINT / SIGTERM，统一退出
- `--headless` 无界面运行（QCoreApplication，不创建主窗口）
- `-s` / `--scenario` 加载 `scenarios/` 下的 JSON 场景
- 仿真步进位于 `SimulationEngine`，GUI 与 headless 共用

`--test` 已出现在 `--help` 中，已支持 `--headless --test`：默认执行 200 帧后输出 `test_result/report.txt`。退出码 0 表示评估通过且报告保存成功，1 表示评估未通过，2 表示运行错误或测试中断。默认 AEB 期望为 Any；命令行和场景文件尚不能设置 Required / Forbidden。GTest/Qt Test、CTest 和 CI 尚未接入。

---

## 技术栈

- 语言：C++17、Python 3
- GUI：Qt Widgets、Signal/Slot
- 多线程：QThread、moveToThread、Queued Connection
- 构建：CMake 3.16+，Debug / Release，CMake install
- 网络：TCP/IP、QTcpServer、QTcpSocket、JSON 行协议
- Linux：syslog、sigaction、SIGINT、SIGTERM
- 配置：QSettings、INI、QCommandLineParser

---

## 仓库结构

```
ADASim/
├── CMakeLists.txt
├── LICENSE
├── README.md
├── cmake/
│   └── Version.h.in
├── config/
│   └── adasim.ini
├── scenarios/
│   ├── empty.json
│   ├── multi_obstacle.json
│   └── straight_obstacle.json
├── src/
│   ├── algorithm/          # 车模与横纵向控制
│   ├── backend/            # DataLoader / DataManager / Recorder / SimulationEngine
│   ├── communication/      # TCP Server
│   ├── config/
│   ├── gui/
│   ├── headless/           # HeadlessRunner
│   ├── scenario/           # ScenarioLoader
│   ├── system/             # syslog、信号
│   ├── test/               # 仿真评分（不是单元测试）
│   └── main.cpp
└── tools/
    └── python_planner.py
```

---

## 运行架构

```
GUI 模式:
  QApplication -> MainWindow -> SimulationEngine
                                 -> 车模 / 横向控制 / 纵向控制 / Recorder
  DataLoader(QThread) 提供 tick
  SocketServer <--TCP/JSON--> tools/python_planner.py

Headless 模式:
  QCoreApplication -> HeadlessRunner -> SimulationEngine
  同样的 tick / 控制 / 记录
  SIGINT / SIGTERM -> LinuxSignalHandler -> 统一退出
```

v2.3 起仿真核心已放到 `SimulationEngine`。v2.4 起支持 `--headless`。

---

## 命令行

```
./ADASim --help
./ADASim --version
./ADASim --config config/adasim.ini
./ADASim --headless
./ADASim --headless --scenario scenarios/empty.json
./ADASim --headless --scenario scenarios/straight_obstacle.json
```

常用选项：

- `-c` / `--config`  配置文件路径，默认是程序目录下的 `config/adasim.ini`
- `--headless`       无界面运行
- `-s` / `--scenario` 场景 JSON
- `--help` / `--version`

对应实现见 `src/main.cpp`。

---

## 配置文件

默认路径：`config/adasim.ini`

```
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

指定配置：

```
./ADASim --config /path/to/adasim.ini
```

---

## C++ 与 Python Planner 通信

ADASim 作为 TCP Server，Python Planner 作为 TCP Client。

默认监听：`127.0.0.1:8080`

每条 JSON 以换行符结束。

C++ 发出的障碍物消息示例：

```
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

Python Planner 返回示例：

```
{
    "type": "CONTROL",
    "steer_offset": 1.75
}
```

---

## 构建

### Debug

```
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug -j$(nproc)
```

运行：

```
./build-debug/ADASim
./build-debug/ADASim --help
./build-debug/ADASim --version
```

### Release

```
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release -j$(nproc)
```

### 三分钟验证

```
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug -j$(nproc)
./build-debug/ADASim --version
./build-debug/ADASim --headless --scenario scenarios/empty.json
```

`--version` 应为 `2.8.0`。

依赖：CMake 3.16+、Qt5 或 Qt6（Core / Gui / Widgets / Network）、C++17 编译器。

---

## 运行 Python Planner

先启动 ADASim，再启动 Planner：

```
./build-debug/ADASim
python3 tools/python_planner.py
```

两端必须使用同一端口，默认 `8080`。若修改 `adasim.ini` 里的 `planner_port`，Python 脚本也要改成相同端口。

---

## 安装测试

不写入系统目录的本地安装：

```
cmake --install build-release --prefix "$PWD/install"
./install/bin/ADASim
```

安装后结构：

```
install/
└── bin/
    ├── ADASim
    └── config/
        └── adasim.ini
```

---

## Linux 检查命令

```
file build-debug/ADASim
ldd build-debug/ADASim
readelf -h build-debug/ADASim
```

---

## 主要模块

- **DataLoader**：QTimer 按周期产生仿真 Tick，跑在后台 QThread
- **DataManager**：车辆状态、点云、障碍物、Planner 消息流转
- **ObstacleDetector**：根据点云检测障碍物
- **VehicleModel**：简化车辆运动模型
- **TrajectoryController**：按横向误差和航向误差计算转向
- **PurePursuitController**：Pure Pursuit 横向控制
- **LongitudinalController**：目标速度、前方距离、TTC、简化紧急制动
- **SimulationEngine**：仿真步进、控制、记录，GUI 与 headless 共用
- **SimulationRecorder**：仿真帧记录与回放
- **HeadlessRunner**：无窗口启动、运行和收尾
- **ScenarioLoader**：读取 `scenarios/*.json`
- **SocketServer**：QTcpServer / QTcpSocket，与 Python Planner 通信
- **ConfigManager**：QSettings 读写 INI
- **LinuxLogger**：Linux 下 syslog
- **LinuxSignalHandler**：sigaction 接收 SIGINT / SIGTERM，经 Qt 事件循环安全退出
- **TestEvaluator**（`src/test/`）：仿真结果评分，不是单元测试框架

---

## 场景文件

- `scenarios/empty.json`
- `scenarios/straight_obstacle.json`
- `scenarios/multi_obstacle.json`

---

## 后续计划

- **v2.9**：接通 `--test` 退出码、GTest、CI
- **v3.0**：通信健壮性、POSIX/UDS、systemd

---

## 许可证

本项目使用 MIT License，见根目录 `LICENSE`。
