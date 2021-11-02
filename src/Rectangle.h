#ifndef PENDULUM_RECTANGLE_H
#define PENDULUM_RECTANGLE_H

#include <SDL2/SDL.h>

// UI rectangle element
// Just a rectangle with a color
// Alpha channel is nice btw.
class Rectangle
{
public:
    Rectangle(const SDL_Rect& rect, const SDL_Color& color);

    SDL_Rect rect();
    void setRect(const SDL_Rect& rect);
    void setColor(const SDL_Color& color);

    void render(SDL_Renderer* renderer) const;
    void render(SDL_Renderer* renderer, const SDL_Color& override) const;

    SDL_Color color();

private:
    SDL_Rect rect_;
    SDL_Color color_;
};

#endif // PENDULUM_RECTANGLE_H
