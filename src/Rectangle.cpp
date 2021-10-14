#include "Rectangle.h"

Rectangle::Rectangle(const SDL_Rect& rect, const SDL_Color& color)
    : rect_(rect)
    , color_(color)
{
}

void
Rectangle::render(SDL_Renderer* renderer) const
{
    SDL_SetRenderDrawColor(renderer, color_.r, color_.g, color_.b, color_.a);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(renderer, &rect_);
}