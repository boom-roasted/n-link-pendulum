#ifndef CHAIN_H
#define CHAIN_H

#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

struct Pin
{
    double x;
    double y;

    Pin(double x, double y)
        : x(x)
        , y(y)
    {
    }
};

// Vector in the spacial 2D sense of the word
struct Vector
{
    double x;
    double y;

    Vector(double x, double y)
        : x(x)
        , y(y)
    {
    }

    double Dot(const Vector& other) const { return x * other.x + y * other.y; }

    double Magnitude() const { return std::sqrt(x * x + y * y); }

    Vector operator*(double k) const { return Vector(x * k, y * k); }

    Vector ProjectOnto(const Vector& other) const
    {
        return other * (Dot(other) / other.Dot(other));
    }
};

inline Vector
operator*(double k, const Vector& v)
{
    return v * k;
}

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

    State operator*(double k) const
    {
        return State(x * k, y * k, xdot * k, ydot * k);
    }

    State operator+(const State& other) const
    {
        return State(
            x + other.x, y + other.y, xdot + other.xdot, ydot + other.ydot);
    }
};

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

    Node()
        : Node(0, 0, 0, 0, 0, 0, 0, 0)
    {
    } // Requried for reading binary file

    Node WithState(const State& newState) const
    {
        return Node(m, l, k, c, newState);
    }
};

inline std::vector<State>
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

inline std::vector<Node>
operator+(const std::vector<Node>& nodes, const std::vector<State>& states)
{
    if (nodes.size() != states.size())
        throw std::runtime_error(
            "Cannot add state list to node list of different sizes");

    auto s = std::vector<Node>();
    s.reserve(nodes.size());

    for (std::size_t i = 0; i < nodes.size(); i++)
    {
        s.push_back(nodes[i].WithState(nodes[i].state + states[i]));
    }

    return s;
}

inline std::vector<State>
operator+(
    const std::vector<State>& statesLHS,
    const std::vector<State>& statesRHS)
{
    if (statesLHS.size() != statesRHS.size())
        throw std::runtime_error("Cannot add state lists of different sizes");

    auto s = std::vector<State>();
    s.reserve(statesLHS.size());

    for (std::size_t i = 0; i < statesLHS.size(); i++)
    {
        s.push_back(statesLHS[i] + statesRHS[i]);
    }

    return s;
}

inline std::vector<State>
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

class Chain
{
    double ts_; // Time stamp
    Pin pin_;
    std::vector<Node> nodes;

    Chain(double ts, const Pin& pin, const std::vector<Node>& nodes)
        : ts_(ts)
        , pin_(pin)
        , nodes(nodes)
    {
    }

public:
    enum class Layout
    {
        Line,
        LShape,
    };

    static Chain
    Create(int numNodes, double m, double l, double k, double c, Layout layout)
    {
        // Defaults
        const double currentTime = 0;
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

        return Chain(currentTime, pin, nodes);
    }

    // A second order Runge Kutta function
    void RungeKuttaSecondOrder(double deltaT)
    {
        // Update time
        ts_ += deltaT;

        const auto z1 = nodes;
        const auto f1 = ComputeState(pin_, z1);

        const auto deltaZ1 = f1 * deltaT;

        const auto z2 = z1 + deltaZ1;
        const auto f2 = ComputeState(pin_, z2);

        // Update node states
        for (std::size_t n = 0; n < nodes.size(); n++)
        {
            auto& node = nodes[n];

            // Function evaluation at specific node
            const auto& f1n = f1[n];
            const auto& f2n = f2[n];

            node.state.x += 0.5 * deltaT * (f1n.x + f2n.x);
            node.state.y += 0.5 * deltaT * (f1n.y + f2n.y);
            node.state.xdot += 0.5 * deltaT * (f1n.xdot + f2n.xdot);
            node.state.ydot += 0.5 * deltaT * (f1n.ydot + f2n.ydot);
        }
    }

    // Serialize this chain in it's current state to binary format
    void Serialize(std::ofstream& f)
    {
        // Current time
        f.write(reinterpret_cast<char*>(&ts_), sizeof(ts_));

        // Pin x, pin y
        f.write(reinterpret_cast<char*>(&pin_.x), sizeof(pin_.x));
        f.write(reinterpret_cast<char*>(&pin_.y), sizeof(pin_.y));

        // Number of nodes
        auto numNodes = nodes.size();
        f.write(reinterpret_cast<char*>(&numNodes), sizeof(numNodes));

        // This should work because vector data should be contiguous.
        // Point to the first item, then write out the size of all items.
        f.write(
            reinterpret_cast<char*>(&nodes[0]),
            nodes.size() * sizeof(nodes[0])); // Node data
    }

    // Read in a binary file of many chains
    static std::vector<Chain> Deserialize(const std::string& p)
    {
        // Open file for reading
        auto f = std::ifstream(p, std::ios::in | std::ios::binary);
        if (!f)
            throw std::runtime_error("Cannot open file for reading");

        // Initialize chain vector
        auto chains = std::vector<Chain>();

        // Read all chains available
        while (!f.eof())
        {
            // Current time
            double currentTime;
            f.read(reinterpret_cast<char*>(&currentTime), sizeof(currentTime));

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

            // Store chain
            chains.push_back(Chain(currentTime, Pin(xPin, yPin), nodes));
        }

        // Close input file
        f.close();

        // TODO The last value appears to be trash.
        chains.pop_back();

        return chains;
    }

    void PrintState() const
    {
        std::cout << "t = " << ts_;

        for (std::size_t n = 0; n < nodes.size(); n++)
        {
            const auto& node = nodes[n];
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
};

#endif // CHAIN_H