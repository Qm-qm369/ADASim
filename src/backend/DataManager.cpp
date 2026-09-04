#include "DataManager.h"

#include <cmath>

DataManager::DataManager(QObject *parent)
    : QObject(parent)
{
    // 40米以内，至少5个点形成一个有效障碍物
    obstacleDetector_.setThreshold(
        40.0f,
        5);
}

DataManager::~DataManager()
{
}

void DataManager::updateVehicleState(
    double x,
    double y,
    double yaw)
{

    // 保存最新车辆状态
    vehicleX_ = x;
    vehicleY_ = y;
    vehicleYaw_ = yaw;

    // 转发给其他模块

    emit vehicleStateUpdated(
        vehicleX_,
        vehicleY_,
        vehicleYaw_);
}

void DataManager::onVehiclePositionReceived(
    double x,
    double y,
    double yaw)
{
    // 保存当前自车位姿
    lastEgoX_ = x;
    lastEgoY_ = y;
    lastEgoYaw_ = yaw;

    // =====================================
    // 1. 保存历史轨迹
    // =====================================

    historyTrajectory_.append(
        QPointF(x, y));

    // 最多保存200帧
    if (historyTrajectory_.size() > 200)
    {
        historyTrajectory_.pop_front();
    }

    // =====================================
    // 2. 预测未来3秒
    // =====================================

    PredictedPath predicted =
        pathPredictor_.predict(
            historyTrajectory_,
            3);

    // =====================================
    // 3. 发布预测结果
    // =====================================

    emit pathPredicted(
        predicted.path);
}

void DataManager::onUserObstacleAdded(
    double x,
    double y)
{
    userObstacles_.append(
        QPointF(x, y));
}

// 接收一帧雷达点云，把用户在沙盒里手动放置的障碍物伪装成雷达点加入进去，然后调用 ObstacleDetector 检测障碍物，最后把结果分别发给 View2D 和 SensorView。
// 它实际上就是一条完整的点云感知流水线。
// 把用户在世界地图中放置的“真实障碍物”，模拟成车辆雷达能看到的点云，再经过 ObstacleDetector 检测，最后把检测结果重新映射回世界地图。

void DataManager::onPointCloudReceived(
    const QVector<QPointF> &points)
{
    // 先复制 DataLoader 提供的基础点云
    QVector<QPointF> mergedPoints = points;

    // 当前帧航向角只计算一次
    const double cosYaw = std::cos(lastEgoYaw_);

    const double sinYaw = std::sin(lastEgoYaw_);

    // =====================================
    // 1. 把真值障碍物模拟成雷达点
    // =====================================

    for (const QPointF &obstacle : userObstacles_)
    {
        // 障碍物相对车辆的全局差值
        double dx = obstacle.x() - lastEgoX_;

        double dy = obstacle.y() - lastEgoY_;

        // 全局坐标 -> 自车局部坐标
        double localX = dx * cosYaw + dy * sinYaw;

        double localY = -dx * sinYaw + dy * cosYaw;

        // 只模拟40米以内目标
        if (localX * localX + localY * localY >= 1600.0)
        {
            continue;
        }

        // 围绕障碍物中心生成36个雷达反射点
        for (int i = 0; i < 36; ++i)
        {
            double angle = i * 10.0 * M_PI / 180.0;

            mergedPoints.append(
                QPointF(
                    localX + 0.5 * std::cos(angle),

                    localY + 0.5 * std::sin(angle)));
        }
    }

    // =====================================
    // 2. 点云 -> 障碍物
    // =====================================

    QVector<Obstacle> obstacles =
        obstacleDetector_.detect(
            mergedPoints);

    QVector<QPointF> obstaclePositions;

    // =====================================
    // 3. 检测结果局部坐标 -> 世界坐标
    // =====================================

    for (const Obstacle &obstacle : obstacles)
    {
        double globalX =
            lastEgoX_ + obstacle.position.x() * cosYaw - obstacle.position.y() * sinYaw;

        double globalY =
            lastEgoY_ + obstacle.position.x() * sinYaw + obstacle.position.y() * cosYaw;

        obstaclePositions.append(
            QPointF(
                globalX,
                globalY));
    }

    // View2D 使用全局障碍物位置
    emit obstaclesDetected(
        obstaclePositions);

    // SensorView 使用自车局部点云 看到的是类似车辆雷达第一视角的数据。
    emit mergedPointCloudReady(
        mergedPoints);
}