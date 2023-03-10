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
public:
    enum class Layout
    {
        Line,
        LShape,
    };

    std::vector<Node> nodes() const { return nodes_; }
    Pin pin() const { return pin_; }

    static Pendulum
    Create(int numNodes, double m, double l, double k, double c, Layout layout);

    // A second order Runge Kutta function
    void ApplyRungeKuttaSecondOrder(double deltaT);

    // Serialize this Pendulum in it's current state to binary format.
    void Serialize(std::ofstream& f);

    // Read a Pendulum from a binary file.
    static Pendulum Deserialize(std::ifstream& f);

    void PrintState() const;

private:
    Pendulum(const Pin& pin, const std::vector<Node>& nodes)
        : pin_(pin)
        , nodes_(nodes)
    {
    }

    Pin pin_;
    std::vector<Node> nodes_;
};
}

#endif // PENDULUM_PENDULUM_H
