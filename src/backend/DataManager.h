/*
 类似于数据总线思想 这就是个中间层
DataLoader 产生数据
        ↓
DataManager 保存数据
        ↓
MainWindow 显示
*/

#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QVector>
#include <QPointF>

#include "algorithm/ObstacleDetector.h"

class DataManager : public QObject
{
    Q_OBJECT

public:
    explicit DataManager(QObject *parent = nullptr);
    ~DataManager();

signals:

    void obstaclesDetected(
        const QVector<QPointF> &obstaclePositions);

    void mergedPointCloudReady(
        const QVector<QPointF> &points);

    // 给 UI 和算法发送最新车辆状态
    void vehicleStateUpdated(
        double x,
        double y,
        double yaw);

public slots:
    void onPointCloudReceived(const QVector<QPointF> &points);

    // 接收 DataLoader 数据
    void updateVehicleState(
        double x,
        double y,
        double yaw);

    void onUserObstacleAdded(
        double x,
        double y);

    void onVehiclePositionReceived(
        double x,
        double y,
        double yaw);

private:
    // 点云聚类算法
    ObstacleDetector obstacleDetector_;

    // 用户右键添加的真值障碍物，全局坐标
    QVector<QPointF> userObstacles_;

    // V0.7车辆状态
    // 保存 DataLoader 传进来的当前车辆状态，然后再通过 vehicleStateUpdated 转发出去。
    double vehicleX_ = 0.0;
    double vehicleY_ = 0.0;
    double vehicleYaw_ = 0.0;

    // V0.8感知计算使用的当前自车位姿
    double lastEgoX_ = 0.0;
    double lastEgoY_ = 0.0;
    double lastEgoYaw_ = 0.0;
};

#endif