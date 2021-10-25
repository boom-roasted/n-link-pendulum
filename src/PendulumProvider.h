#ifndef PENDULUM_PENDULUMPROVIDER_H
#define PENDULUM_PENDULUMPROVIDER_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>

#include "Pendulum.h"
#include "Texture.h"

class PendulumProvider
{
public:
    PendulumProvider(const SDL_Rect& rect);

    void setRect(const SDL_Rect& rect);

    bool loadFromFile(const std::string& p);
    void loadTextures(SDL_Renderer* renderer);

    Pendulum::Pendulum currentPendulum();

    // Timing controls
    void restart();
    void incrementFrame(std::size_t by);
    void decrementFrame(std::size_t by);
    void incrementTime(double by);

    // Zooming commands
    void zoomFit();
    void zoom(double factor);

    void render(SDL_Renderer* renderer);

private:
    SDL_Rect rect_;

    Pendulum::OverTime pendulumOverTime_;

    Texture pinTexture_;

    Texture nodeTexture_;

    std::size_t currentPendulumIndex_;
    std::size_t lastFrame_;

    double scaleFactor_;

    int xOrigin();
    int yOrigin();

    // Computes the ideal scaling factor to fit the simulation in bounds
    // No-op if pendulum is not loaded
    void computeScaleFactor();

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

#endif // PENDULUM_PENDULUMPROVIDER_H
