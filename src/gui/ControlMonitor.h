// 控制曲线监控

#ifndef CONTROLMONITOR_H
#define CONTROLMONITOR_H

#include <QWidget>
#include <QVector>

class QPainter;

class ControlMonitor : public QWidget
{
    Q_OBJECT

public:
    explicit ControlMonitor(QWidget *parent = nullptr);

public slots:
    void appendSample(
        double lateralError,
        double steeringAngle);

    void clear();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawSeries(
        QPainter &painter,
        const QVector<double> &values,
        const QRectF &rect,
        double range);

private:
    QVector<double> lateralErrors_;
    QVector<double> steeringDegrees_;

    int maxSamples_ = 120;
};

#endif