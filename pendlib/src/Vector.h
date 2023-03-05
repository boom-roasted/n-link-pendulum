#ifndef PENDULUM_VECTOR_H
#define PENDULUM_VECTOR_H

#include <cmath>
#include <vector>

#include "State.h"

namespace pendlib
{
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
}

#endif // PENDULUM_VECTOR_H
