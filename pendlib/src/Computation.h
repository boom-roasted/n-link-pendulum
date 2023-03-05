#ifndef PENDULUM_COMPUTATION_H
#define PENDULUM_COMPUTATION_H

#include <vector>

#include "Pin.h"
#include "Node.h"
#include "State.h"

namespace pendlib
{
std::vector<State>
ComputeState(const Pin& pin, const std::vector<Node>& nodes);
}

#endif // PENDULUM_COMPUTATION_H
