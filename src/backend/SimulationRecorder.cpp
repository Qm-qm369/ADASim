#include "SimulationRecorder.h"

SimulationRecorder::SimulationRecorder()
{
}

void SimulationRecorder::clear()
{
    frames_.clear();
}

void SimulationRecorder::append(const SimulationFrame &frame)
{
    frames_.append(frame);

    if (frames_.size() > maxFrames_)
    {
        frames_.pop_front();
    }
}

int SimulationRecorder::size() const
{
    return frames_.size();
}

bool SimulationRecorder::frameAt(int index, SimulationFrame &frame) const
{
    if (index < 0 || index >= frames_.size())
    {
        return false;
    }

    frame = frames_[index];
    return true;
}