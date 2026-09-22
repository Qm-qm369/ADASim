#include "HeadlessRunner.h"

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QTimer>
#include <QSaveFile>

#include "backend/DataLoader.h"
#include "backend/DataManager.h"
#include "communication/Socket.h"
#include "system/LinuxLogger.h"

// 启动一个无界面模式的 ADASim，把配置、数据时钟、数据管理、仿真核心、TCP 网络这些后台模块创建起来并连接好。
HeadlessRunner::HeadlessRunner(
    const QString &configPath,
    const QString &dataPath,
    const QString &scenarioPath,
    QObject *parent)
    : QObject(parent),
      configPath_(configPath),
      dataPath_(dataPath),
      scenarioPath_(scenarioPath)
{
}

HeadlessRunner::~HeadlessRunner()
{
    shutdown();

    LinuxLogger::info(
        "ADASim headless mode stopped");

    LinuxLogger::shutdown();
}

bool HeadlessRunner::initialize()
{
    if (initialized_)
    {
        return true;
    }

    LinuxLogger::init();

    LinuxLogger::info(
        "ADASim headless mode initializing");

    // 1. 加载配置
    if (!loadConfig())
    {
        return false;
    }

    // 2. 创建数据时钟
    dataLoader_ =
        new DataLoader(
            dataPath_,
            this);

    // 3. 创建数据管理模块
    dataManager_ =
        new DataManager(this);

    // 4. 创建仿真核心
    simulationEngine_ =
        new SimulationEngine(this);

    // 使用 INI 参数配置 Engine
    simulationEngine_->configure(
        appConfig_.targetSpeed,
        appConfig_.planningDistance,
        appConfig_.lookAheadDistance,
        appConfig_.headingGain,
        appConfig_.lateralGain,
        appConfig_.controllerMode ==
            "PurePursuit");

    // 5. 创建 TCP Server
    socketServer_ =
        new SocketServer(this);

    // 6. 建立连接
    setupConnections();

    if (!loadScenario())
    {
        LinuxLogger::warning(
            "No valid scenario loaded");

        return false;
    }

    if (!setupNetwork())
    {
        return false;
    }

    if (!simulationEngine_->startRecording("record/simulation.csv"))
    {
        qCritical() << "[INIT] Cannot start recording";
        return false;
    }

    LinuxLogger::info(
        "ADASim headless initialized");

    initialized_ = true;

    return true;
}

bool HeadlessRunner::loadConfig()
{
    QString errorMessage;

    AppConfig loadedConfig;

    bool success =
        ConfigManager::load(
            configPath_,
            loadedConfig,
            errorMessage);

    if (success)
    {
        appConfig_ =
            loadedConfig;

        LinuxLogger::info(
            QString(
                "Config loaded: %1")
                .arg(configPath_));

        return true;
    }
    else
    {
        LinuxLogger::warning(
            errorMessage);

        qCritical().noquote() << "[CONFIG]" << errorMessage;
        return false;
    }
}

bool HeadlessRunner::loadScenario()
{
    // 没有指定scenario
    // 保持V2.4行为
    if (scenarioPath_.isEmpty())
    {
        qInfo()
            << "[SCENARIO]"
            << "No scenario specified";

        return true;
    }

    QString errorMessage;

    bool success =
        ScenarioLoader::load(
            scenarioPath_,
            scenario_,
            errorMessage);

    if (!success)
    {
        LinuxLogger::error(
            errorMessage);

        qCritical().noquote()
            << "[SCENARIO]"
            << errorMessage;

        return false;
    }

    qInfo().noquote()
        << QString(
               "[SCENARIO] Loaded: %1 "
               "obstacles=%2")
               .arg(
                   scenario_.name)
               .arg(
                   scenario_.obstacles.size());

    for (const QPointF &point :
         scenario_.obstacles)
    {
        qInfo()
            << "[SCENARIO]"
            << "Obstacle:"
            << point.x()
            << point.y();

        dataManager_->onUserObstacleAdded(
            point.x(),
            point.y());
    }

    scenarioLoaded_ = true;

    return true;
}

void HeadlessRunner::setupConnections()
{
    // ==========================================
    // DataLoader -> SimulationEngine
    // ==========================================

    connect(
        dataLoader_,
        &DataLoader::simulationTick,
        simulationEngine_,
        &SimulationEngine::onSimulationTick);

    // ==========================================
    // SimulationEngine -> DataManager
    // ==========================================

    connect(
        simulationEngine_,
        &SimulationEngine::simulationFrameReady,
        dataManager_,
        &DataManager::onSimulationFrame);

    // ==========================================
    // Engine状态 -> Headless终端
    // ==========================================

    connect(
        simulationEngine_,
        &SimulationEngine::frameUpdated,
        this,
        &HeadlessRunner::onSimulationFrameUpdated);

    // ==========================================
    // DataManager -> Engine
    // ==========================================

    connect(
        dataManager_,
        &DataManager::frontObstacleDistanceUpdated,
        simulationEngine_,
        &SimulationEngine::onFrontObstacleDistanceUpdated);

    connect(
        dataManager_,
        &DataManager::lateralControlReceived,
        simulationEngine_,
        &SimulationEngine::onLateralControlReceived);

    // ==========================================
    // DataLoader 状态
    // ==========================================

    connect(
        dataLoader_,
        &DataLoader::statusUpdate,
        this,
        &HeadlessRunner::onStatusUpdate);

    // ==========================================
    // Planner协议错误
    // ==========================================

    connect(
        dataManager_,
        &DataManager::plannerMessageError,
        this,
        [](const QString &message)
        {
            LinuxLogger::warning(
                message);

            qWarning().noquote()
                << "[PLANNER]"
                << message;
        });

    connect(
        dataManager_,
        &DataManager::plannerDataReady,
        socketServer_,
        &SocketServer::sendToClient);

    connect(
        socketServer_,
        &SocketServer::dataReceived,
        dataManager_,
        &DataManager::onPlannerDataReceived);

    connect(
        socketServer_,
        &SocketServer::networkError,
        this,
        [](const QString &message)
        {
            LinuxLogger::error(
                message);

            qCritical().noquote()
                << "[NETWORK]"
                << message;
        });

    connect(
        socketServer_,
        &SocketServer::clientConnected,
        this,
        []()
        {
            LinuxLogger::info(
                "Python Planner connected");

            qInfo()
                << "[NETWORK]"
                << "Python Planner connected";
        });

    connect(
        socketServer_,
        &SocketServer::clientDisconnected,
        this,
        []()
        {
            LinuxLogger::warning(
                "Python Planner disconnected");

            qWarning()
                << "[NETWORK]"
                << "Python Planner disconnected";
        });
}

bool HeadlessRunner::setupNetwork()
{
    if (appConfig_.plannerPort < 1 || appConfig_.plannerPort > 65535)
    {
        qCritical() << "[NETWORK] Invalid port:" << appConfig_.plannerPort;
        return false;
    }

    bool success =
        socketServer_->startTcpServer(
            static_cast<quint16>(
                appConfig_.plannerPort));

    if (success)
    {
        LinuxLogger::info(
            QString(
                "Planner TCP Server listening on %1")
                .arg(
                    appConfig_.plannerPort));

        qInfo()
            << "[NETWORK]"
            << "Planner TCP Server:"
            << appConfig_.plannerPort;
    }
    else
    {
        LinuxLogger::error(
            "Planner TCP Server start failed");

        qCritical()
            << "[NETWORK]"
            << "TCP Server start failed";
    }

    return success;
}

void HeadlessRunner::start()
{
    if (started_)
    {
        return;
    }

    if (!initialized_)
    {
        qCritical()
            << "[ADASim]"
            << "Runner not initialized";

        return;
    }

    started_ = true;

    LinuxLogger::info(
        "ADASim headless simulation started");

    qInfo()
        << "[ADASim]"
        << "Headless simulation started";

    // 先初始化 Engine
    simulationEngine_->start();

    // 再启动100ms Tick
    dataLoader_->start();
}

void HeadlessRunner::onSimulationFrameUpdated(
    const SimulationFrame &frame,
    int currentIndex,
    bool targetSettled)
{
    Q_UNUSED(currentIndex);

    ++frameCounter_;

    if (testMode_ &&
        frameCounter_ >= maxTestFrames_ &&
        !testFinished_)
    {
        testFinished_ = true;

        dataLoader_->pause();

        QTimer::singleShot(0, this, [this]()
                           { finishTest(); });
    }

    // DataLoader是10Hz
    // 每10帧打印一次，大约每秒一次
    if (frameCounter_ % 10 != 0)
    {
        return;
    }

    QString controllerName =
        frame.controllerMode == 0
            ? "DualError"
            : "PurePursuit";

    qInfo().noquote()
        << QString(
               "[SIM] x=%1 m  y=%2 m  "
               "speed=%3 km/h  "
               "controller=%4  "
               "TTC=%5 s  "
               "AEB=%6")
               .arg(
                   frame.vehicle.x,
                   0,
                   'f',
                   2)
               .arg(
                   frame.vehicle.y,
                   0,
                   'f',
                   2)
               .arg(
                   frame.speedKmH,
                   0,
                   'f',
                   1)
               .arg(
                   controllerName)
               .arg(
                   frame.ttc,
                   0,
                   'f',
                   2)
               .arg(
                   frame.emergencyBrake
                       ? "ON"
                       : "OFF");

    if (targetSettled)
    {
        qInfo()
            << "[SIM]"
            << "Lateral target settled";
    }
}

void HeadlessRunner::onStatusUpdate(
    const QString &status)
{
    qInfo().noquote()
        << "[STATUS]"
        << status;
}
void HeadlessRunner::onTerminationRequested(
    int signalNumber)
{
    LinuxLogger::warning(
        QString(
            "Termination signal received: %1")
            .arg(signalNumber));

    qInfo()
        << "[SYSTEM]"
        << "Termination signal:"
        << signalNumber;

    shutdown();

    if (testMode_)
    {
        qCritical() << "[TEST] Aborted before completion";
        QCoreApplication::exit(2);
    }
    else
    {
        QCoreApplication::quit();
    }
}

void HeadlessRunner::shutdown()
{
    if (shutdownStarted_)
        return;

    shutdownStarted_ = true;

    if (dataLoader_)
        dataLoader_->stop();

    if (simulationEngine_)
        simulationEngine_->stop();

    if (socketServer_)
        socketServer_->stop();
}

void HeadlessRunner::setTestMode(
    bool enable)
{
    testMode_ = enable;
}

bool HeadlessRunner::saveTestReport(const TestResult &result)
{
    if (!QDir().mkpath("test_result")) // 在当前工作目录下创建 test_result，父目录不存在也会一起建。
    {
        qCritical() << "[TEST] Cannot create test_result directory"; // qCritical() 是 Qt 的严重错误日志宏，用来打印已经出错、但程序通常还能继续的信息
        return false;
    }

    QSaveFile file("test_result/report.txt"); // QSaveFile 不是直接改原文件，而是先写临时文件，最后 commit() 再原子替换
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qCritical() << "[TEST] Cannot open report:" << file.errorString();
        return false;
    }

    QTextStream out(&file); // 写入报告内容
    out << "ADASim Test Report\n\n"
        << "PASS:" << result.passed << "\n"
        << "Frames:" << result.frameCount << "\n"
        << "AEB:" << result.aebTriggered << "\n"
        << "Collision:" << result.collision << "\n"
        << "Min TTC:" << result.minTtc << "\n";

    out.flush(); // flush() 把缓冲区刷下去。 检查写入是否成功
    if (out.status() != QTextStream::Ok)
    {
        qCritical() << "[TEST] Cannot write report:" << file.errorString();
        file.cancelWriting();
        return false;
    }

    if (!file.commit()) // commit() 把临时文件替换成 test_result/report.txt
    {
        qCritical() << "[TEST] Cannot commit report:" << file.errorString();
        return false;
    }

    qInfo().noquote() << "[TEST] Report saved:"
                      << QDir().absoluteFilePath("test_result/report.txt");
    return true;
}

void HeadlessRunner::finishTest()
{
    if (shutdownStarted_) // 1. 防止重复收尾
        return;

    const TestResult result =
        simulationEngine_->testResult(aebExpectation_);
    const bool reportSaved = saveTestReport(result);

    shutdown();

    if (!reportSaved)
    {
        qCritical() << "[TEST] ERROR: report was not saved";
        QCoreApplication::exit(2);
    }
    else
    {
        qInfo() << "[TEST]" << (result.passed ? "PASS" : "FAIL");
        QCoreApplication::exit(result.passed ? 0 : 1);
    }
}