#ifndef PENDULUM_PIN_H
#define PENDULUM_PIN_H

namespace pendlib
{
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
}

#endif // PENDULUM_PIN_H
