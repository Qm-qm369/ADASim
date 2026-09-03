#ifndef DATALOADER_H
#define DATALOADER_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QThread>
#include <QVector>
#include <QVector>
#include <QPointF>

class DataLoader : public QObject
{
    Q_OBJECT

public:
    // explicit 禁止构造函数的隐式类型转换，只允许显式调用构造。只对单参数（或者除第一个参数外都有默认值）**的构造函数生效。
    explicit DataLoader(const QString &dataPath,
                        QObject *parent = nullptr);

    ~DataLoader();

signals:

    // 当前帧 LiDAR 点云
    void pointCloudReady(const QVector<QPointF> &points);
    // 向外发送车辆位置
    void vehiclePositionReady(double x, double y, double yaw);

    // 向界面发送运行状态
    void statusUpdate(const QString &status);

    // 通知 UI：当前历史缓存一共有多少帧
    void totalFramesLoaded(int total);

    // 通知 UI：现在位于第几帧
    void currentFrameUpdated(int index);

public slots:
    void start();
    void pause();
    void stop();

    // 跳转到指定历史帧
    void seekToFrame(int index);

private slots:
    // 定时器每次触发时产生一帧数据
    void loadNextFrame();

private:
    // 一帧车辆状态
    struct FrameState
    {
        double x;
        double y;
        double yaw;
    };

    // 最近100帧历史记录
    QVector<FrameState> historyQueue_;

    // 沙盒真实世界中的 X 坐标
    double liveSandboxX_ = 0.0;

    /*
     * -1 ：正常生成未来帧
     * >=0：正在历史回放
     */
    int replayCursor_ = -1;

    QString dataPath_;

    bool isRunning_ = false;

    QTimer *timer_ = nullptr;
};

#endif // DATALOADER_H