#ifndef PENDULUM_FPSCOUNTER_H
#define PENDULUM_FPSCOUNTER_H

#include <SDL2/SDL.h>
#include <sstream>

#include "Texture.h"
#include "Timer.h"

class FpsCounter
{
public:
    FpsCounter(SDL_Rect rect);

    void setRect(const SDL_Rect& rect);
    void start();

    // Render the fps counter to a corner of the window
    // This assumes it will be called for every render frame
    void render(SDL_Renderer* renderer, TTF_Font* font);

private:
    SDL_Rect rect_;
    Timer timer_;
    Texture texture_;
    SDL_Color color_;
    std::stringstream text_;
    unsigned long long countedFrames_;
};

#endif // PENDULUM_FPSCOUNTER_H
