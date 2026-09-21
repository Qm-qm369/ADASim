#include "HeadlessRunner.h"

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QTextStream>

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

    simulationEngine_->startRecording(
        "record/simulation.csv");

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

    setupNetwork();

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

void HeadlessRunner::setupNetwork()
{
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

    QCoreApplication::quit();
}

void HeadlessRunner::shutdown()
{
    TestResult result =
        simulationEngine_->testResult();

    qInfo()
        << "[TEST]"
        << "AEB:"
        << result.aebTriggered
        << "Collision:"
        << result.collision
        << "Min TTC:"
        << result.minTtc;

    if (shutdownStarted_)
    {
        return;
    }

    shutdownStarted_ = true;

    LinuxLogger::info(
        "ADASim headless shutdown started");

    if (dataLoader_)
    {
        dataLoader_->stop();
    }

    if (simulationEngine_)
    {
        simulationEngine_->stop();
    }

    if (socketServer_)
    {
        socketServer_->stop();
    }

    if (testMode_)
    {
        printTestResult();
    }

    LinuxLogger::info(
        "ADASim headless shutdown completed");
}

void HeadlessRunner::setTestMode(
    bool enable)
{
    testMode_ = enable;
}

void HeadlessRunner::printTestResult()
{

    TestResult result =
        simulationEngine_->testResult();

    qInfo()
        << "========== TEST ==========";

    qInfo()
        << "AEB:"
        << result.aebTriggered;

    qInfo()
        << "Collision:"
        << result.collision;

    qInfo()
        << "Min TTC:"
        << result.minTtc;

    qInfo()
        << "==========================";
}

void HeadlessRunner::saveTestReport(
    const TestResult &result)
{

    QFile file(
        "test_result/report.txt");

    if (!file.open(
            QIODevice::WriteOnly |
            QIODevice::Text))
    {
        return;
    }

    QTextStream out(&file);

    out
        << "ADASim Test Report\n\n";

    out
        << "PASS:"
        << result.passed
        << "\n";

    out
        << "AEB:"
        << result.aebTriggered
        << "\n";

    out
        << "Collision:"
        << result.collision
        << "\n";

    out
        << "Min TTC:"
        << result.minTtc
        << "\n";

    QDir dir(
        "test_result");

    if (!dir.exists())
    {
        dir.mkpath(".");
    }

    file.close();
}