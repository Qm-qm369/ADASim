#ifndef TESTEVALUATOR_H
#define TESTEVALUATOR_H

#include <QObject>

#include "backend/SimulationRecorder.h"

struct TestResult
{
    bool aebTriggered = false;

    bool collision = false;

    double minTtc = -1.0;

    bool passed = false;
};

class TestEvaluator : public QObject
{
    Q_OBJECT

public:
    explicit TestEvaluator(
        QObject *parent = nullptr);

    void reset();

    void processFrame(
        const SimulationFrame &frame);

    TestResult result() const;

private:
    TestResult result_;
};

#endif