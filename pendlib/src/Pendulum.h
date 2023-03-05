#ifndef PENDULUM_PENDULUM_H
#define PENDULUM_PENDULUM_H

#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "Pin.h"
#include "Node.h"
#include "State.h"
#include "Computation.h"
#include "Operators.h"

namespace pendlib
{
class Pendulum
{
    double ts_; // Time stamp
    Pin pin_;
    std::vector<Node> nodes_;

    Pendulum(double ts, const Pin& pin, const std::vector<Node>& nodes)
        : ts_(ts)
        , pin_(pin)
        , nodes_(nodes)
    {
    }

public:
    enum class Layout
    {
        Line,
        LShape,
    };

    double time() const { return ts_; }
    std::vector<Node> nodes() const { return nodes_; }
    Pin pin() const { return pin_; }

    static Pendulum
    Create(int numNodes, double m, double l, double k, double c, Layout layout);

    // A second order Runge Kutta function
    void RungeKuttaSecondOrder(double deltaT);

    // Serialize this Pendulum in it's current state to binary format
    void Serialize(std::ofstream& f);

    // Read in a binary file of many Pendulums
    static std::vector<Pendulum> Deserialize(const std::string& p);

    void PrintState() const;
};

using OverTime = std::vector<Pendulum>;
}

#endif // PENDULUM_PENDULUM_H
