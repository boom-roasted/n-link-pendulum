#ifndef PENDULUM_STRING_H
#define PENDULUM_STRING_H

#include <string>

inline std::string str(const char* s)
{
    return std::string(s);
}

inline std::string str(int x)
{
    return std::to_string(x);
}

#endif // PENDULUM_STRING_H
