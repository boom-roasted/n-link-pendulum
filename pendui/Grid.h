#ifndef PENDULUM_GRID_H
#define PENDULUM_GRID_H

#include <SDL2/SDL.h>

class Grid
{
public:
    Grid(
        const SDL_Rect& rect,
        const SDL_Point& origin,
        const SDL_Color& color,
        int spacingH,
        int spacingV,
        double scaleFactor);

    SDL_Rect rect();
    void setRect(const SDL_Rect& rect);

    void setOrigin(const SDL_Point& origin);
    void setScaleFactor(double sf);

    void render(SDL_Renderer* renderer);
    void renderAxes(SDL_Renderer* renderer);

private:
    SDL_Rect rect_;
    SDL_Point origin_;
    SDL_Color color_;
    int spacingH_;
    int spacingV_;
    double scaleFactor_;
};

#endif // PENDULUM_GRID_H
