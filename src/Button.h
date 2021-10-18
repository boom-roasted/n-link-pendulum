#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <string>

#include "Text.h"

class Button
{
public:
    Button(
        const SDL_Rect& rect,
        int id,
        const std::string& text,
        const SDL_Color& fgColor,
        const SDL_Color& bgColor);

    void setRect(const SDL_Rect& rect);

    void render(SDL_Renderer* renderer, TTF_Font* font);

    // Tracks mouse events. Will start tracking mouse motion
    // if mouse button is down.
    void handleEvent(SDL_Event& e);

    // Click event only counts if the mouse if both pressed
    // AND released within the button's bounds
    bool wasClicked();

    int id();
    std::string text();

private:
    bool isInBounds(int x, int y);

    SDL_Rect rect_;
    int id_;
    Text text_;
    bool isPressed_;
    bool isClicked_;
};

#endif