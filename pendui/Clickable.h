#ifndef PENDULUM_CLICKABLE_H
#define PENDULUM_CLICKABLE_H

#include <SDL2/SDL.h>
#include <vector>

#include "Hotkey.h"

class Clickable
{
public:
    Clickable(const SDL_Rect& rect);
    Clickable(const SDL_Rect& rect, const Hotkey& hotkey);
    Clickable(const SDL_Rect& rect, const std::vector<Hotkey>& hotkeys);

    void setRect(const SDL_Rect& rect);

    // Tracks mouse events. Will start tracking mouse motion
    // if mouse button is down.
    void handleEvent(SDL_Event& e);

    // Click event only counts if the mouse if both pressed
    // AND released within the button's bounds
    bool wasClicked();

    bool isPressed();

private:
    bool isInBounds(int x, int y);

    SDL_Rect rect_;
    bool isPressed_;
    bool isClicked_;
    std::vector<Hotkey> hotkeys_;
};

#endif // PENDULUM_CLICKABLE_H
