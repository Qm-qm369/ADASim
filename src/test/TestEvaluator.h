#ifndef TESTEVALUATOR_H
#define TESTEVALUATOR_H

#include <QVector>

#include "backend/SimulationRecorder.h"

struct TestResult
{

    bool passed = false;

    bool aebTriggered = false;

    bool collision = false;

    double minTtc = -1.0;
};

class TestEvaluator
{

public:
    TestResult evaluate(
        const QVector<SimulationFrame> &frames);
};

#endif