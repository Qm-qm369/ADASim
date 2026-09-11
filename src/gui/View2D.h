#ifndef VIEW2D_H
#define VIEW2D_H

#include <QWidget>
#include <QVector>
#include <QPointF>
#include <QPolygonF>

class QPainter;
class QMouseEvent;

class QPainter;

/**
 * @brief ADASim 二维俯视图
 */
class View2D : public QWidget
{
    Q_OBJECT

public:
    explicit View2D(QWidget *parent = nullptr);
    ~View2D();

public slots:
    // 接收 PathPredictor 计算出的未来轨迹
    void updatePredictedPath(
        const QVector<QPointF> &path);

    void updateObstacles(
        const QVector<QPointF> &obstacles);

    // 接收新的车辆位姿
    void updateVehiclePosition(double x,
                               double y,
                               double yaw);
    void drawTrajectory(QPainter &painter);

    // V0.10 接收Python选择的横向目标
    void setPlannedOffset(double offset);

    void updatePlannedTrajectory(const QVector<QPointF> &trajectory);

signals:
    void userObstacleAdded(
        double worldX,
        double worldY);

protected:
    // QWidget 需要重绘时，Qt 会自动调用
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(
        QMouseEvent *event) override;

private:
    void drawObstacles(
        QPainter &painter);

    // 绘制背景网格
    void drawMap(QPainter &painter);

    // 绘制自车
    void drawVehicle(QPainter &painter);

    // 绘制未来预测轨迹
    void drawPredictedPath(QPainter &painter);

    // 绘制Lattice候选轨迹
    void drawPlanning(QPainter &painter);

private:
    // PathPredictor预测出的未来世界坐标
    QVector<QPointF> predictedPath_;
    // 自车过去走过的轨迹
    QPolygonF trajectory_;

    // 感知算法检测出来的障碍物
    QVector<QPointF> obstacles_;

    // 用户右键放置的真实障碍物
    QVector<QPointF> globalUserObstacles_;

    // 像素/米，用于把物理尺寸转换成屏幕尺寸
    double zoom_ = 20.0;

    // 当前车辆全局位姿
    double vehicleX_ = 0.0;
    double vehicleY_ = 0.0;
    double vehicleYaw_ = 0.0;

    // Python当前选择的横向偏移
    double plannedOffset_ = 0.0;

    // 是否已经收到过规划结果
    bool planningActive_ = false;

    QVector<QPointF> plannedTrajectory_;
};

#endif // VIEW2D_H