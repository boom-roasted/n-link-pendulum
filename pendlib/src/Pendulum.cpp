#include "Pendulum.h"

#include "Pin.h"
#include "Node.h"
#include "State.h"
#include "Computation.h"
#include "Operators.h"

namespace pendlib
{
Pendulum
Pendulum::Create(int numNodes, double m, double l, double k, double c, Layout layout)
{
    // Defaults
    const auto pin = Pin(0, 0);
    const double xdot0 = 0;
    const double ydot0 = 0;

    // Generate nodes
    auto nodes = std::vector<Node>();
    nodes.reserve(numNodes);

    for (int n = 0; n < numNodes; n++)
    {
        double x0;
        double y0;

        switch (layout)
        {
            case Layout::Line:
                x0 = pin.x + l * (n + 1);
                y0 = pin.y;
                break;

            case Layout::LShape:
                throw std::runtime_error("Not implemented");
            default:
                throw std::runtime_error("Invalid layout option");
        }

        const auto node = Node(m, l, k, c, x0, y0, xdot0, ydot0);
        nodes.push_back(node);
    }

    return Pendulum(pin, nodes);
}

void Pendulum::ApplyRungeKuttaSecondOrder(double deltaT)
{
    const auto z1 = nodes_;
    const auto f1 = ComputeState(pin_, z1);

    const auto deltaZ1 = f1 * deltaT;

    const auto z2 = z1 + deltaZ1;
    const auto f2 = ComputeState(pin_, z2);

    // Update node states
    for (std::size_t n = 0; n < nodes_.size(); n++)
    {
        auto& node = nodes_[n];

        // Function evaluation at specific node
        const auto& f1n = f1[n];
        const auto& f2n = f2[n];

        node.state.x += 0.5 * deltaT * (f1n.x + f2n.x);
        node.state.y += 0.5 * deltaT * (f1n.y + f2n.y);
        node.state.xdot += 0.5 * deltaT * (f1n.xdot + f2n.xdot);
        node.state.ydot += 0.5 * deltaT * (f1n.ydot + f2n.ydot);
    }
}

void Pendulum::Serialize(std::ofstream& f)
{
    // Pin x, pin y
    f.write(reinterpret_cast<char*>(&pin_.x), sizeof(pin_.x));
    f.write(reinterpret_cast<char*>(&pin_.y), sizeof(pin_.y));

    // Number of nodes
    auto numNodes = nodes_.size();
    f.write(reinterpret_cast<char*>(&numNodes), sizeof(numNodes));

    // This should work because vector data should be contiguous.
    // Point to the first item, then write out the size of all items.
    f.write(
        reinterpret_cast<char*>(&nodes_[0]),
        nodes_.size() * sizeof(nodes_[0])); // Node data
}

Pendulum Pendulum::Deserialize(std::ifstream& f)
{
    // Pin x, pin y
    double xPin;
    double yPin;
    f.read(reinterpret_cast<char*>(&xPin), sizeof(xPin));
    f.read(reinterpret_cast<char*>(&yPin), sizeof(yPin));

    // Number of nodes
    std::size_t numNodes;
    f.read(reinterpret_cast<char*>(&numNodes), sizeof(numNodes));

    // Nodes vector
    std::vector<Node> nodes(numNodes);
    f.read(
        reinterpret_cast<char*>(nodes.data()),
        nodes.size() * sizeof(Node));

    // Done
    return Pendulum(Pin(xPin, yPin), nodes);
}

void Pendulum::PrintState() const
{
    for (std::size_t n = 0; n < nodes_.size(); n++)
    {
        const Node& node = nodes_[n];
        std::cout << "\t"
                    << "Node(x: " << node.state.x << ", y: " << node.state.y
                    <<
            // ", xdot: " << node.state.xdot << ", ydot: " <<
            // node.state.ydot
            // <<
            ")";
    }

    std::cout << std::endl;
}
}
