#ifndef SIMULATIONRECORDER_H
#define SIMULATIONRECORDER_H

#include <QVector>

#include "algorithm/VehicleModel.h"

struct SimulationFrame
{
    VehicleState vehicle;

    double targetY = 0.0;
    double targetYaw = 0.0;

    double lateralError = 0.0;
    double headingError = 0.0;

    double steeringAngle = 0.0;

    double speedKmH = 0.0;
    double totalDistance = 0.0;

    int controllerMode = 0;

    // V1.8纵向控制
    double targetSpeedKmH = 0.0;
    double acceleration = 0.0;

    double frontObstacleDistance = -1.0;

    double ttc = -1.0;

    bool emergencyBrake = false;
};

class SimulationRecorder
{
public:
    SimulationRecorder();

    void clear();

    void append(const SimulationFrame &frame);

    int size() const;

    bool frameAt(int index, SimulationFrame &frame) const;

private:
    QVector<SimulationFrame> frames_;

    // 10Hz下保存最近30秒
    int maxFrames_ = 300;
};

#endif