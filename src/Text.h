#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <string>

#include "Rectangle.h"
#include "Texture.h"

class Text
{
public:
    Text(
        const SDL_Rect& rect,
        const SDL_Color& fgColor,
        const SDL_Color& bgColor,
        const std::string& text);

    void render(SDL_Renderer* renderer, TTF_Font* font);

    std::string text();

private:
    SDL_Rect rect_;
    Rectangle background_;
    SDL_Color fgColor_;
    Texture textTexture_;
    std::string text_;
};

#endif // TEXT_H