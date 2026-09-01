#ifndef VIEW2D_H
#define VIEW2D_H

#include <QWidget>

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
    // 接收新的车辆位姿
    void updateVehiclePosition(double x,
                               double y,
                               double yaw);

protected:
    // QWidget 需要重绘时，Qt 会自动调用
    void paintEvent(QPaintEvent *event) override;

private:
    // 绘制背景网格
    void drawMap(QPainter &painter);

    // 绘制自车
    void drawVehicle(QPainter &painter);

private:
    // 像素/米，用于把物理尺寸转换成屏幕尺寸
    double zoom_ = 20.0;

    // 当前车辆全局位姿
    double vehicleX_ = 0.0;
    double vehicleY_ = 0.0;
    double vehicleYaw_ = 0.0;
};

#endif // VIEW2D_H