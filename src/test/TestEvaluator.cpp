#include "TestEvaluator.h"

TestResult TestEvaluator::evaluate(const QVector<SimulationFrame> &frames)
{

    TestResult result;

    double minTtc = -1.0;

    for (const auto &frame : frames)
    {

        if (frame.emergencyBrake)
        {
            result.aebTriggered = true;
        }

        if (frame.ttc > 0)
        {

            if (minTtc < 0 ||
                frame.ttc < minTtc)
            {
                minTtc = frame.ttc;
            }
        }

        if (frame.frontObstacleDistance < 0)
        {
            result.collision = true;
        }
    }

    result.minTtc = minTtc;

    result.passed =
        result.aebTriggered &&
        !result.collision;

    return result;
}