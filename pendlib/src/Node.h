#ifndef PENDULUM_NODE_H
#define PENDULUM_NODE_H

#include "State.h"

namespace pendlib
{
struct Node
{
    double m; // Mass of node, kg
    double l; // Initial length to node, meter
    double k; // Spring stiffness, N/m
    double c; // Dampening, N/m/s

    State state; // State variables

    Node(double m, double l, double k, double c, const State& state)
        : m(m)
        , l(l)
        , k(k)
        , c(c)
        , state(state)
    {
    }

    Node(
        double m,
        double l,
        double k,
        double c,
        double x0,
        double y0,
        double xdot0,
        double ydot0)
        : m(m)
        , l(l)
        , k(k)
        , c(c)
        , state(x0, y0, xdot0, ydot0)
    {
    }

    // Requried for reading binary file
    Node()
        : Node(0, 0, 0, 0, 0, 0, 0, 0)
    {
    }

    Node WithState(const State& newState) const
    {
        return Node(m, l, k, c, newState);
    }
};
}

#endif // PENDULUM_NODE_H
