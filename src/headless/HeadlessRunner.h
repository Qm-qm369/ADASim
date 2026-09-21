#ifndef HEADLESSRUNNER_H
#define HEADLESSRUNNER_H

#include <QObject>
#include <QString>

#include "config/ConfigManager.h"
#include "scenario/ScenarioLoader.h"
#include "backend/SimulationEngine.h"

// 前向声明。只需要知道有这么一个类 暂时不需要类里面长什么样
class DataLoader;
class DataManager;
class SocketServer;

class HeadlessRunner : public QObject
{
    Q_OBJECT

public:
    explicit HeadlessRunner(
        const QString &configPath,
        const QString &dataPath,
        const QString &scenarioPath,
        QObject *parent = nullptr);

    ~HeadlessRunner();

    // V2.4 新增：
    // 启动 Headless 仿真
    void start();

    bool initialize();

    void setTestMode(
        bool enable);

public slots:

    // LinuxSignalHandler 发来 SIGINT / SIGTERM 后执行
    void onTerminationRequested(
        int signalNumber);

private slots:

    // Engine 每计算完一帧后，用于终端显示
    void onSimulationFrameUpdated(
        const SimulationFrame &frame,
        int currentIndex,
        bool targetSettled);

    // DataLoader 状态输出
    void onStatusUpdate(
        const QString &status);

private:
    bool loadConfig();
    bool loadScenario();

    void setupConnections();

    void setupNetwork();

    void shutdown();

    void printTestResult();

    void saveTestReport(const TestResult &result);

private:
    QString configPath_;
    QString dataPath_;
    QString scenarioPath_;

    AppConfig appConfig_;
    Scenario scenario_;

    DataLoader *dataLoader_ = nullptr;

    DataManager *dataManager_ = nullptr;

    SimulationEngine *simulationEngine_ = nullptr;

    SocketServer *socketServer_ = nullptr;

    bool initialized_ = false;
    bool started_ = false;

    bool shutdownStarted_ = false;

    bool scenarioLoaded_ = false;

    // 控制终端打印频率
    int frameCounter_ = 0;

    bool testMode_ = false;
};

#endif