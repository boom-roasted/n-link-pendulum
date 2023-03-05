#include "Computation.h"

#include <vector>

#include "Pin.h"
#include "Node.h"
#include "State.h"
#include "Operators.h"
#include "Vector.h"

namespace pendlib
{
std::vector<State>
ComputeState(const Pin& pin, const std::vector<Node>& nodes)
{
    // Initialize return structure
    auto states = std::vector<State>();
    states.reserve(nodes.size());

    // Compute state for each node.
    // Reverse list to avoid double-computing link forces.
    // Link force (in the /next/ direction) for the last node is 0 since there
    // is only one link connected.
    double xForceNext = 0.0;
    double yForceNext = 0.0;

    for (int n = nodes.size() - 1; n >= 0; --n)
    {
        const auto& node = nodes[n];

        // The first node is connected to the pin rather than another node
        const double xPrev = n == 0 ? pin.x : nodes[n - 1].state.x;
        const double yPrev = n == 0 ? pin.y : nodes[n - 1].state.y;

        // Distance from this node to the previous one
        const auto dist = std::sqrt(
            (node.state.x - xPrev) * (node.state.x - xPrev) +
            (node.state.y - yPrev) * (node.state.y - yPrev));

        // Difference in distance from initial link length
        const auto deltaS = dist - node.l;

        // Spring force of link on node (Hooke's Law)
        const auto kForce = node.k * deltaS;

        // Velocity of link expansion is the portion of velocity along the link
        const auto linkExpansionV =
            Vector(node.state.xdot, node.state.ydot)
                .ProjectOnto(
                    Vector(node.state.x - xPrev, node.state.y - yPrev));

        // Damper force of link on node
        const auto cForce = (node.c * linkExpansionV).Magnitude();

        // Force of link on node
        const auto force = kForce - cForce;

        // Deconstruct force into x and y directions
        const auto xForce = force * (node.state.x - xPrev) / dist;
        const auto yForce = force * (node.state.y - yPrev) / dist;

        // Acceleration
        const auto xdotdot = 1.0 / node.m * (-xForce + xForceNext);
        const auto ydotdot = 1.0 / node.m * (-yForce + yForceNext) + 9.81;

        // Save the computed x and y forces for the next node to use
        xForceNext = xForce;
        yForceNext = yForce;

        // Assign updated state variables
        states.push_back(
            State(node.state.xdot, node.state.ydot, xdotdot, ydotdot));
    }

    return states;
}
}