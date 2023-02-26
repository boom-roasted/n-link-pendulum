#ifndef PENDULUM_STRING_H
#define PENDULUM_STRING_H

#include <string>

std::string str(const char* s)
{
    return std::string(s);
}

std::string str(int x)
{
    return std::to_string(x);
}

#endif // PENDULUM_STRING_H
