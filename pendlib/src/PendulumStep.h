#ifndef PENDULUM_PENDULUMSTEP_H
#define PENDULUM_PENDULUMSTEP_H

#include "Pendulum.h"

namespace pendlib
{
class PendulumStep
{
public:
    PendulumStep(Pendulum& pend)
        : PendulumStep(0.0, pend)
    {
    }

    void IncrementTime(double deltaT);

    void Serialize(std::ofstream& f);
    static PendulumStep Deserialize(std::ifstream& f, double time);
    static std::vector<PendulumStep> DeserializeTimeSeries(const std::string& p);

    double time() const { return ts_; }
    Pendulum pendulum() const { return pend_; }

private:
    PendulumStep(double ts, Pendulum& pend)
        : ts_(ts)
        , pend_(pend)
    {
    }

    Pendulum pend_;
    double ts_; // Time stamp
};

using PendulumTimeSeries = std::vector<PendulumStep>;
}


#endif // PENDULUM_PENDULUMSTEP_H
