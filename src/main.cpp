#include <QApplication>
#include <QString>
#include <QCommandLineParser> //Qt命令行参数解析器
#include <QCoreApplication>

#include "gui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setApplicationName("ADASim");

    QCoreApplication::setApplicationVersion("1.9");

    // =====================================
    // V1.9：命令行参数
    // =====================================

    /*
    QCommandLineParser 是 Qt 自带类。  解析用户启动程序时，在后面写的这些参数。
    ./ADASim --config abc.ini
    ./ADASim --help
    ./ADASim --version
    */
    QCommandLineParser parser;

    parser.setApplicationDescription("ADASim autonomous driving simulator");

    parser.addHelpOption();
    parser.addVersionOption();

    // QCoreApplication::applicationDirPath()  是 Qt 自带函数。获取当前 ADASim 可执行程序所在的目录。
    QString defaultConfigPath = QCoreApplication::applicationDirPath() + "/config/adasim.ini";

    // 创建一个 -c 或 --config 命令行选项，用于指定配置文件；如果用户没指定，就使用默认的 config/adasim.ini。
    QCommandLineOption configOption(
        QStringList() << "c" << "config",
        "ADASim config file",
        "file",
        defaultConfigPath);

    parser.addOption(configOption);

    parser.process(app); // 真正去检查用户启动程序时写了什么。

    QString configPath = parser.value(configOption);

    QString dataPath;

    MainWindow mainWindow(configPath, dataPath);

    mainWindow.show();

    return app.exec();
}