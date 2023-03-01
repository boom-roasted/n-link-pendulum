#ifndef PENDULUM_HOTKEY_H
#define PENDULUM_HOTKEY_H

#include <SDL2/SDL.h>

class Hotkey
{
public:
    Hotkey(SDL_KeyCode keycode);
    Hotkey(SDL_KeyCode keycode, SDL_Keymod modifiers);

    bool isActive(SDL_Event& e) const;

private:
    SDL_KeyCode keycode_;
    SDL_Keymod modifiers_ = SDL_Keymod::KMOD_NONE; // Can be OR'd together
};

#endif // PENDULUM_HOTKEY_H
