#include "DataLoader.h"

DataLoader::DataLoader(const QString &dataPath,
                       QObject *parent)
    : QObject(parent), dataPath_(dataPath), replayCursor_(-1), isRunning_(false), timer_(new QTimer(this))
{
    // QVector<QPointF> 后面需要跨线程传输
    // 把 QVector<QPointF> 这种数据类型登记到 Qt 的“类型系统”里，让 Qt 知道这种类型。
    qRegisterMetaType<QVector<QPointF>>("QVector<QPointF>");

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
    connect(timer_, &QTimer::timeout,
            this, &DataLoader::loadNextFrame);
}

DataLoader::~DataLoader()
{
    stop();
}

// 实现时光倒流函数
void DataLoader::seekToFrame(int index)
{
    if (index < 0 ||
        index >= historyQueue_.size())
    {
        return;
    }

    replayCursor_ = index;

    FrameState state = historyQueue_[index];

    // 更新车辆位置
    emit vehiclePositionReady(
        state.x,
        state.y,
        state.yaw);

    // 同时重新触发当前帧的感知计算
    emit pointCloudReady(QVector<QPointF>());

    emit currentFrameUpdated(index);
}

/**
 * 启动仿真
 */
void DataLoader::start()
{
    qDebug() << "DataLoader::start thread ="
             << QThread::currentThread();
    if (QThread::currentThread() != this->thread())
    {
        return;
    }

    if (!isRunning_)
    {
        isRunning_ = true;
        timer_->start(100);

        emit statusUpdate("正在运行：仿真推演中...");
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

    if (timer_->isActive())
    {
        timer_->stop();
    }

    // 清空所有历史状态
    historyQueue_.clear();

    liveSandboxX_ = 0.0;

    replayCursor_ = -1;

    // 重置时间轴
    emit totalFramesLoaded(0);

    emit currentFrameUpdated(0);

    emit statusUpdate(
        "引擎已停止并重置");
}

/**
 * 每100ms生成一帧车辆状态
 */
void DataLoader::loadNextFrame()
{
    if (!isRunning_)
        return;

    // ======================================
    // 模式1：正在播放历史缓存
    // ======================================

    if (replayCursor_ >= 0 &&
        replayCursor_ < historyQueue_.size() - 1)
    {
        // 播放下一帧历史数据
        replayCursor_++;

        FrameState state =
            historyQueue_[replayCursor_];

        emit vehiclePositionReady(
            state.x,
            state.y,
            state.yaw);

        // 历史回放时也重新计算雷达数据
        emit pointCloudReady(QVector<QPointF>());

        // 更新 UI 时间轴位置
        emit currentFrameUpdated(replayCursor_);

        return;
    }

    // ======================================
    // 模式2：已经追上最新时间，生成未来
    // ======================================

    replayCursor_ = -1;

    // 沙盒车辆继续向前
    liveSandboxX_ += 0.5;

    FrameState state{
        liveSandboxX_,
        0.0,
        0.0};

    // 保存新产生的一帧
    historyQueue_.append(state);

    // FIFO：最多保存100帧
    if (historyQueue_.size() > 100)
    {
        historyQueue_.pop_front();
    }

    // 分发车辆状态
    emit vehiclePositionReady(
        state.x,
        state.y,
        state.yaw);

    // 发出一帧基础点云
    emit pointCloudReady(QVector<QPointF>());

    // 通知时间轴当前缓存大小
    emit totalFramesLoaded(historyQueue_.size());

    // 滑块跟随到最新的一帧
    emit currentFrameUpdated(historyQueue_.size() - 1);
}