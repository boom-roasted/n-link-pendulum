#ifndef PENDULUMPROVIDER_H
#define PENDULUMPROVIDER_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>

#include "Pendulum.h"
#include "Texture.h"

class PendulumProvider
{
public:
    PendulumProvider();

    ~PendulumProvider();

    bool loadFromFile(const std::string& p);

    void loadTextures(SDL_Renderer* renderer);

    Pendulum::Pendulum currentPendulum();

    void incrementFrame(std::size_t by);
    void incrementTime(double by);

    void
    render(SDL_Renderer* renderer, double offsetX, double offsetY, double sf);

private:
    Pendulum::OverTime pendulumOverTime;

    Texture pinTexture_;

    Texture nodeTexture_;

    std::size_t currentPendulumIndex_;
    std::size_t lastFrame_;

    struct Point
    {
        double x;
        double y;

        Point(double x, double y)
            : x(x)
            , y(y)
        {
        }
    };
};

#endif // PENDULUMPROVIDER_H