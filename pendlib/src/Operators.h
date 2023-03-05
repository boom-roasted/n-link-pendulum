#ifndef PENDULUM_OPERATORS_H
#define PENDULUM_OPERATORS_H

#include <vector>
#include <stdexcept>

#include "Node.h"
#include "State.h"

namespace pendlib
{
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
}

#endif // PENDULUM_OPERATORS_H
