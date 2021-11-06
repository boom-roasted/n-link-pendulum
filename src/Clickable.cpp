#include "Clickable.h"

Clickable::Clickable(const SDL_Rect& rect, const std::optional<Hotkey>& hotkey)
    : rect_(rect)
    , isPressed_(false)
    , isClicked_(false)
    , hotkey_(hotkey)
{
}

void
Clickable::setRect(const SDL_Rect& rect)
{
    rect_ = rect;
}

void
Clickable::handleEvent(SDL_Event& e)
{
    switch (e.type)
    {
        // When the mouse is down, note that
        case SDL_MOUSEBUTTONDOWN:
            if (isInBounds(e.button.x, e.button.y))
            {
                isPressed_ = true;
            }
            break;

        // When the mouse is released, it is only worthwhile when
        // the mouse was previously pressed
        case SDL_MOUSEBUTTONUP:
            if (isPressed_)
            {
                isPressed_ = false;
                isClicked_ = true;
            }
            break;

        // When the mouse is down, we need to follow it around and see
        // if it stays in this button or if it leaves. If it leaves,
        // we shouldn't generate a click for this button.
        case SDL_MOUSEMOTION:
            if (isPressed_)
            {
                if (!isInBounds(e.motion.x, e.motion.y))
                    isPressed_ = false;
            }
            break;

        // Check if the hotkey is active. If so, generate a normal
        // click event.
        case SDL_KEYUP:
            if (hotkey_)
            {
                if ((*hotkey_).isActive(e))
                {
                    isPressed_ = false;
                    isClicked_ = true;
                }
            }

        default:
            break;
    }
}

bool
Clickable::wasClicked()
{
    const auto result = isClicked_;
    isClicked_ = false; // Reset state now that it's been checked
    return result;
}

bool
Clickable::isPressed()
{
    return isPressed_;
}

bool
Clickable::isInBounds(int x, int y)
{
    // Assume it is inside
    bool inside = true;

    // Mouse is left of the button
    if (x < rect_.x)
        inside = false;

    // Mouse is right of the button
    else if (x > rect_.x + rect_.w)
        inside = false;

    // Mouse above the button
    else if (y < rect_.y)
        inside = false;

    // Mouse below the button
    else if (y > rect_.y + rect_.h)
        inside = false;

    return inside;
}
