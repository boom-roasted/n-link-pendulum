#include "Rectangle.h"

Rectangle::Rectangle(const SDL_Rect& rect, const SDL_Color& color)
    : rect_(rect)
    , color_(color)
{
}

SDL_Rect
Rectangle::rect()
{
    return rect_;
}

void
Rectangle::setRect(const SDL_Rect& rect)
{
    rect_ = rect;
}

void
Rectangle::setColor(const SDL_Color& color)
{
    color_ = color;
}

void
Rectangle::render(SDL_Renderer* renderer) const
{
    render(renderer, color_);
}

void
Rectangle::render(SDL_Renderer* renderer, const SDL_Color& override) const
{
    SDL_SetRenderDrawColor(
        renderer, override.r, override.g, override.b, override.a);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(renderer, &rect_);
}

SDL_Color
Rectangle::color()
{
    return color_;
}