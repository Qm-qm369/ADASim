#include <QApplication>
#include <QString>
#include "gui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString configPath;
    QString dataPath;

    MainWindow mainWindow(configPath, dataPath);

    mainWindow.show();

    return app.exec();
}