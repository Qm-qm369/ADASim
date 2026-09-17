# ADASim

## Automatic Driving Algorithm Simulator

基于 C++17 / Qt / Linux 的自动驾驶算法仿真平台。

ADASim 用于学习和验证自动驾驶基础算法，实现从传感器模拟、障碍物检测、路径规划、车辆控制到可视化显示的完整闭环仿真流程。

---

# 项目展示

## 仿真界面

(这里放截图)

## 避障效果

(这里放截图)

---

# 项目特点

- 基于 Qt 的自动驾驶仿真可视化平台
- 支持 C++ 与 Python Planner TCP 通信
- 实现感知→规划→控制完整闭环
- 支持多线程异步仿真
- 支持参数配置与运行状态保存
- 支持 Linux 应用生命周期管理

---

# 技术栈

## 开发语言

- C++17
- Python

## GUI

- Qt Widgets
- Qt Signal/Slot

## 构建工具

- CMake

## 通信

- TCP/IP
- JSON

## Linux

- pthread/QThread
- syslog
- SIGINT/SIGTERM
- 文件配置管理

---

# 系统架构
