#include "PendulumStep.h"

#include <iostream>

namespace pendlib
{
void PendulumStep::IncrementTime(double deltaT)
{
    // Update time.
    ts_ += deltaT;

    // Apply increment.
    pend_.ApplyRungeKuttaSecondOrder(deltaT);
}

void PendulumStep::Serialize(std::ofstream& f)
{
    // Current time
    f.write(reinterpret_cast<char*>(&ts_), sizeof(ts_));

    // Pendulum
    pend_.Serialize(f);
}

PendulumStep PendulumStep::Deserialize(std::ifstream& f, double time)
{
    // Pendulum data.
    auto pend = Pendulum::Deserialize(f);

    return PendulumStep(time, pend);
}

std::vector<PendulumStep> PendulumStep::DeserializeTimeSeries(const std::string& p)
{
    // Open file for reading
    auto f = std::ifstream(p, std::ios::in | std::ios::binary);
    if (!f)
        throw std::runtime_error("Cannot open file " + p + " for reading");

    // Initialize Pendulum vector
    auto pendulumTimeSeries = std::vector<PendulumStep>();

    // Current time.
    double currentTime;

    // Read all pendulums available
    while (f.read(reinterpret_cast<char*>(&currentTime), sizeof(currentTime)))
    {
        auto pendStep = Deserialize(f, currentTime);

        // Store pendulum
        pendulumTimeSeries.push_back(pendStep);
    }

    // Close input file
    f.close();

    // TODO The last value appears to be trash.
    pendulumTimeSeries.pop_back();

    return pendulumTimeSeries;
}
}