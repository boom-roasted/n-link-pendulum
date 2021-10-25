#ifndef PENDULUM_BUTTONDATA_H
#define PENDULUM_BUTTONDATA_H

#include <string>

template<typename ButtonId>
struct ButtonData
{
    ButtonId id;
    std::string name;

    ButtonData(ButtonId id, const std::string& name)
        : id(id)
        , name(name)
    {
    }
};

#endif // PENDULUM_BUTTONDATA_H
