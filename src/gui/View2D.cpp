#include "View2D.h"

#include <QPainter>
#include <QPen>
#include <QLinearGradient>
#include <cmath>
#include <QMouseEvent>

View2D::View2D(QWidget *parent)
    : QWidget(parent)
{
    // 防止视图被压得太小
    setMinimumSize(400, 400);
}

View2D::~View2D()
{
}

/**
 * paintEvent是QWidget的虚事件函数 属于Qt的事件系统
 * update()并不是直接调用：paintEvent(); 向Qt发出：需要重绘的请求。
 * Qt 需要刷新 View2D 时，会自动调用 paintEvent()
 */
void View2D::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    // 创建画笔，画布就是当前 View2D
    QPainter painter(this);

    // 开启抗锯齿
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制深色背景
    painter.fillRect(rect(), QColor(5, 8, 17));

    // 按顺序绘制
    drawMap(painter);

    // 先画环境
    drawObstacles(painter);

    // 最后画车，保证车辆位于最上层
    drawVehicle(painter);
}

/**
 * 绘制背景网格
 */
void View2D::drawMap(QPainter &painter)
{
    // QPen画笔：专门控制线条的颜色、粗细、样式。
    // A=50：Alpha 透明度，0 完全透明，255 完全不透明。50 代表线条半透明，淡淡的网格，不会抢主体画面
    QPen gridPen(QColor(0, 255, 136, 50));
    gridPen.setWidth(1); // 网格线条宽度 1 像素。

    // 把配置好的笔交给 painter，后续所有drawLine都使用这个半透绿色画笔。
    painter.setPen(gridPen);

    const int gridSize = 50;

    // 根据车辆位置计算网格偏移
    int offsetX = static_cast<int>(vehicleX_ * zoom_) % gridSize;

    int offsetY = static_cast<int>(vehicleY_ * zoom_) % gridSize;

    // 绘制竖线
    for (int x = -offsetX; x < width(); x += gridSize)
    {
        painter.drawLine(x, 0, x, height());
    }

    // 绘制横线
    for (int y = offsetY; y < height(); y += gridSize)
    {
        painter.drawLine(0, y, width(), y);
    }
}

/**
 * 绘制自车
 */
void View2D::drawVehicle(QPainter &painter)
{
    // 保存当前 QPainter 状态
    // restore()：恢复到 save 之前状态。
    painter.save();

    // 与原项目一致：自车放在画面的 1/4 位置
    int screenX = width() / 4;
    int screenY = height() / 2;

    // 把坐标原点移动到汽车中心
    painter.translate(screenX, screenY);

    // 根据航向角旋转汽车
    painter.rotate(-vehicleYaw_ * 180.0 / M_PI);

    // 原源码使用的车辆物理尺寸
    const double physicalLength = 4.6;
    const double physicalWidth = 2.2;

    // 米 -> 像素
    int carLength =
        static_cast<int>(physicalLength * zoom_);

    int carWidth =
        static_cast<int>(physicalWidth * zoom_);

    // =========================
    // 1. 汽车主体
    // =========================

    QLinearGradient carGradient(
        -carLength / 2,
        0,
        carLength / 2,
        0);

    carGradient.setColorAt(0, QColor(220, 220, 230));
    carGradient.setColorAt(1, QColor(160, 170, 180));

    // pen：图形的边框线条- brush：图形内部填充
    painter.setBrush(carGradient);
    painter.setPen(QPen(Qt::white, 1));

    painter.drawRoundedRect(
        -carLength / 2,
        -carWidth / 2,
        carLength,
        carWidth,
        4,
        4);

    // =========================
    // 2. 车头标识
    // =========================
    //
    // 用深色区域表示前挡风玻璃，
    // 这样一眼就能判断车头朝向。

    painter.setBrush(QColor(20, 25, 30));
    painter.setPen(Qt::NoPen);

    painter.drawRect(
        static_cast<int>(carLength * 0.1),
        static_cast<int>(-carWidth * 0.4),
        static_cast<int>(carLength * 0.15),
        static_cast<int>(carWidth * 0.8));

    // 恢复之前的 QPainter 状态
    // 恢复之前的坐标原点等
    painter.restore();
}

void View2D::drawObstacles(
    QPainter &painter)
{
    painter.setPen(Qt::NoPen);

    // =====================================
    // 1. 用户手动添加的真值障碍物
    // =====================================

    painter.setBrush(
        QColor(100, 110, 120, 180));

    for (const QPointF &obstacle : globalUserObstacles_)
    {
        int x =
            width() / 4 +
            (obstacle.x() - vehicleX_) * zoom_;

        int y =
            height() / 2 -
            (obstacle.y() - vehicleY_) * zoom_;

        // 画一个椭圆
        painter.drawEllipse(
            QPoint(x, y),
            6,
            6);
    }

    // =====================================
    // 2. 感知算法检测结果
    // =====================================

    for (const QPointF &obstacle : obstacles_)
    {
        int x =
            width() / 4 +
            (obstacle.x() - vehicleX_) * zoom_;

        int y =
            height() / 2 -
            (obstacle.y() - vehicleY_) * zoom_;

        // 检测范围光晕
        painter.setBrush(
            QColor(255, 50, 50, 100));

        painter.drawEllipse(
            QPoint(x, y),
            10,
            10);

        // 障碍物中心
        painter.setBrush(
            QColor(255, 50, 50));

        painter.drawEllipse(
            QPoint(x, y),
            4,
            4);
    }
}

void View2D::mousePressEvent(
    QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        // 屏幕像素坐标 -> 世界物理坐标
        double worldX =
            vehicleX_ +
            (event->pos().x() - width() / 4.0) / zoom_;

        double worldY =
            vehicleY_ -
            (event->pos().y() - height() / 2.0) / zoom_;

        // View2D自己保存一份真值用于显示
        globalUserObstacles_.append(
            QPointF(
                worldX,
                worldY));

        // 通知DataManager
        emit userObstacleAdded(
            worldX,
            worldY);

        update();
    }

    QWidget::mousePressEvent(event);
}

void View2D::updateVehiclePosition(double x,
                                   double y,
                                   double yaw)
{
    vehicleX_ = x;
    vehicleY_ = y;
    vehicleYaw_ = yaw;

    // 请求 Qt 重新绘制 View2D
    update();
}

void View2D::updateObstacles(
    const QVector<QPointF> &obstacles)
{
    obstacles_ = obstacles;

    update();
}