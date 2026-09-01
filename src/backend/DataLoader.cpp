#include "DataLoader.h"

DataLoader::DataLoader(const QString &dataPath,
                       QObject *parent)
    : QObject(parent), dataPath_(dataPath), timer_(new QTimer(this))
{
    /*
    QObject::connect(
    const QObject *sender,        // 参数1：信号发送者对象指针
    const char *signal,           // 参数2：信号（函数地址）
    const QObject *receiver,     // 参数3：接收者对象指针
    const char *method,          // 参数4：槽函数（函数地址）
    Qt::ConnectionType type = Qt::AutoConnection // 参数5：连接类型（可选，大部分情况不用写）
);

    */
    // QTimer 每次 timeout 都执行 loadNextFrame()
    connect(timer_,
            &QTimer::timeout,
            this,
            &DataLoader::loadNextFrame);
}

DataLoader::~DataLoader()
{
    stop();
}

/**
 * 启动仿真
 */
void DataLoader::start()
{
    if (!isRunning_)
    {
        isRunning_ = true;

        // 100ms 一帧，也就是 10Hz
        timer_->start(100);

        emit statusUpdate("仿真运行中");
    }
}

/**
 * 暂停仿真
 */
void DataLoader::pause()
{
    if (isRunning_)
    {
        isRunning_ = false;
        timer_->stop();

        emit statusUpdate("仿真已暂停");
    }
}

/**
 * 停止并重置
 */
void DataLoader::stop()
{
    isRunning_ = false;

    timer_->stop();

    // 从世界坐标 X=0 重新开始
    liveSandboxX_ = 0.0;

    emit statusUpdate("仿真已停止");
}

/**
 * 每100ms生成一帧车辆状态
 */
void DataLoader::loadNextFrame()
{
    if (!isRunning_)
        return;

    // 原源码中的简易沙盒运动模型
    liveSandboxX_ += 0.5;

    double x = liveSandboxX_;
    double y = 0.0;
    double yaw = 0.0;

    // 把这一帧车辆位置发出去
    emit vehiclePositionReady(x, y, yaw);
}