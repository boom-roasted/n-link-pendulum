#ifndef PENDULUM_TEXT_H
#define PENDULUM_TEXT_H

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
        const std::string& text,
        SDL_Renderer* renderer,
        TTF_Font* font);

    void setRect(const SDL_Rect& rect);

    void setIsPressed();

    void setText(const std::string& text);

    void render();

    std::string text();

private:
    void swapFgBgNextRender();
    void loadTexture();
    void swapFgBg();

    SDL_Rect rect_;
    Rectangle background_;
    SDL_Color fgColor_;
    Texture textTexture_;
    std::string text_;
    SDL_Renderer* renderer_;
    TTF_Font* font_;
    bool isPressed_;
    bool isDirty_; // Whether or not to re-load the textures
};

#endif // PENDULUM_TEXT_H
