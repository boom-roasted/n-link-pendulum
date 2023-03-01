#include "Hotkey.h"

Hotkey::Hotkey(SDL_KeyCode keycode)
    : keycode_(keycode)
{
}

Hotkey::Hotkey(SDL_KeyCode keycode, SDL_Keymod modifiers)
    : keycode_(keycode)
    , modifiers_(modifiers)
{
}

bool
Hotkey::isActive(SDL_Event& e) const
{
    return (
        e.type == SDL_KEYDOWN &&            // On the key down
        e.key.repeat == 0 &&                // Don't bother when key is held
        !(e.key.keysym.mod & modifiers_) && // Modifiers match
        e.key.keysym.sym == keycode_        // Key matches
    );
}