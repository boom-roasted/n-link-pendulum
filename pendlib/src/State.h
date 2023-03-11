#ifndef PENDULUM_STATE_H
#define PENDULUM_STATE_H

#include <vector>

namespace pendlib
{
// Position and velocity.
struct State
{
    double x; // X position
    double y; // Y position

    double xdot; // X velocity
    double ydot; // Y velocity

    State(double x, double y, double xdot, double ydot)
        : x(x)
        , y(y)
        , xdot(xdot)
        , ydot(ydot)
    {
    }

    State operator*(double k) const;

    State operator+(const State& other) const;
};

std::vector<State>
operator*(const std::vector<State>& states, double k);
}

#endif // PENDULUM_STATE_H
