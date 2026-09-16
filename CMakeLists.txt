cmake_minimum_required(VERSION 3.16)

project(ADASim VERSION 1.0.0 LANGUAGES CXX)

# C++ 标准与 Qt 自动生成工具配置
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 查找 Qt (向下兼容 Qt5，向上支持 Qt6)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets OpenGL Network Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets OpenGL Network Sql)

# 查找 OpenCV 视觉库
find_package(OpenCV REQUIRED)

# ============================================================================
# [源文件清单]
# ============================================================================
set(PROJECT_SOURCES
    src/main.cpp

    src/gui/MainWindow.cpp
    src/gui/View2D.cpp
    src/gui/SensorView.cpp
    src/gui/ControlMonitor.cpp

    src/backend/DataLoader.cpp
    src/backend/DataManager.cpp
    src/backend/SimulationRecorder.cpp

    src/algorithm/ObstacleDetector.cpp
    src/algorithm/PathPredictor.cpp
    src/algorithm/VehicleModel.cpp
    src/algorithm/TrajectoryController.cpp
    src/algorithm/PurePursuitController.cpp
    src/algorithm/LongitudinalController.cpp

    src/communication/Socket.cpp

    src/system/LinuxLogger.cpp

    src/config/ConfigManager.cpp
)

# ==========================================
# V1.9：把默认配置文件复制到构建目录
# ==========================================

file( MAKE_DIRECTORY
    ${CMAKE_BINARY_DIR}/config
)

configure_file(
    ${CMAKE_SOURCE_DIR}/config/adasim.ini
    ${CMAKE_BINARY_DIR}/config/adasim.ini
    COPYONLY
)

# 创建可执行文件
add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
)

# 链接第三方库
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGL
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Sql
    ${OpenCV_LIBS}
)

# 包含头文件目录 (Include Directories)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/gui
    ${CMAKE_SOURCE_DIR}/src/backend
    ${OpenCV_INCLUDE_DIRS}
)

# 安装部署规则
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)