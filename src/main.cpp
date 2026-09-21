#include <QApplication>
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QString>

#include <memory>

#include "gui/MainWindow.h"
#include "headless/HeadlessRunner.h"
#include "system/LinuxSignalHandler.h"
#include "Version.h"

// 在创建 QApplication / QCoreApplication 之前
// 先判断用户有没有传 --headless。
static bool hasHeadlessArgument(
    int argc,
    char *argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        // 把 C 风格字符串 char * 转成 Qt 的 QString。
        if (QString::fromLocal8Bit(
                argv[i]) ==
            "--headless")
        {
            return true;
        }
    }

    return false;
}

int main(
    int argc,
    char *argv[])
{
    // ==========================================
    // V2.4：决定使用GUI还是Headless应用
    // ==========================================

    bool useHeadlessApplication =
        hasHeadlessArgument(
            argc,
            argv);

    std::unique_ptr<QCoreApplication> app; // 创建了一个智能指针

    // make_unique主要目的就是安全地创建 unique_ptr
    if (useHeadlessApplication)
    {
        app =
            std::make_unique<QCoreApplication>(
                argc,
                argv);
    }
    else
    {
        app =
            std::make_unique<QApplication>(
                argc,
                argv);
    }

    QCoreApplication::setApplicationName(
        "ADASim");

    QCoreApplication::setApplicationVersion(
        ADASIM_VERSION);

    // ==========================================
    // 创建命令行解析器
    // ==========================================

    QCommandLineParser parser;

    parser.setApplicationDescription(
        "ADASim autonomous driving simulator");

    parser.addHelpOption();

    parser.addVersionOption();

    // ==========================================
    // --config
    // ==========================================

    QString defaultConfigPath =
        QCoreApplication::applicationDirPath() +
        "/config/adasim.ini";

    QCommandLineOption configOption(
        QStringList()
            << "c"
            << "config",
        "ADASim config file",
        "file",
        defaultConfigPath);

    parser.addOption(
        configOption);

    // ==========================================
    // V2.4：正式注册 --headless
    // ==========================================

    QCommandLineOption headlessOption(
        QStringList()
            << "headless",
        "Run ADASim without GUI");

    parser.addOption(
        headlessOption);

    // 正式解析命令行
    parser.process(
        *app);

    QString configPath =
        parser.value(
            configOption);

    bool headlessMode =
        parser.isSet(
            headlessOption);

    QString dataPath;

    // ==========================================
    // Linux SIGINT / SIGTERM
    // ==========================================

    LinuxSignalHandler signalHandler;

    signalHandler.install();

    // ==========================================
    // Headless模式
    // ==========================================

    if (headlessMode)
    {
        HeadlessRunner runner(
            configPath,
            dataPath);

        QObject::connect(
            &signalHandler,
            &LinuxSignalHandler::terminationRequested,
            &runner,
            &HeadlessRunner::onTerminationRequested);

        runner.start();

        // 进入 Qt 事件循环。
        return app->exec();
    }

    // ==========================================
    // GUI模式
    // ==========================================

    MainWindow mainWindow(
        configPath,
        dataPath);

    QObject::connect(
        &signalHandler,
        &LinuxSignalHandler::terminationRequested,
        &mainWindow,
        &MainWindow::onTerminationRequested);

    mainWindow.show();

    return app->exec();
}