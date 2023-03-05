#include "State.h"

namespace pendlib
{
State State::operator*(double k) const
{
    return State(x * k, y * k, xdot * k, ydot * k);
}

State State::operator+(const State& other) const
{
    return State(
        x + other.x, y + other.y, xdot + other.xdot, ydot + other.ydot);
}

std::vector<State>
operator*(const std::vector<State>& states, double k)
{
    auto s = std::vector<State>();
    s.reserve(states.size());

    for (const auto& state : states)
    {
        s.push_back(state * k);
    }

    return s;
}
}