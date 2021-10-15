#include "Button.h"

Button::Button(
    const SDL_Rect& rect,
    const SDL_Color& fgColor,
    const SDL_Color& bgColor,
    const std::string& text)
    : rect_(rect)
    , text_(Text(rect, fgColor, bgColor, text))
    , wasPressed_(false)
    , wasClicked_(false)
{
}

void
Button::render(SDL_Renderer* renderer, TTF_Font* font)
{
    text_.render(renderer, font);
}

void
Button::handleEvent(SDL_Event& e)
{
    switch (e.type)
    {
        // When the mouse is down, note that
        case SDL_MOUSEBUTTONDOWN:
            if (isInBounds(e.button.x, e.button.y))
            {
                SDL_LogDebug(
                    SDL_LOG_CATEGORY_INPUT,
                    "button %s mouse down at x=%d, y=%d",
                    text().c_str(),
                    e.button.x,
                    e.button.y);
                wasPressed_ = true;
            }
            break;

        // When the mouse is released, it is only worthwhile when
        // the mouse was previously pressed
        case SDL_MOUSEBUTTONUP:
            if (wasPressed_)
            {
                SDL_LogDebug(
                    SDL_LOG_CATEGORY_INPUT,
                    "button %s mouse up at x=%d, y=%d",
                    text().c_str(),
                    e.button.x,
                    e.button.y);
                wasPressed_ = false;
                wasClicked_ = true;
            }
            break;

        // When the mouse is down, we need to follow it around and see
        // if it stays in this button or if it leaves. If it leaves,
        // we shouldn't generate a click for this button.
        case SDL_MOUSEMOTION:
            if (wasPressed_)
            {
                SDL_LogDebug(
                    SDL_LOG_CATEGORY_INPUT,
                    "button %s mouse move at x=%d, y=%d",
                    text().c_str(),
                    e.motion.x,
                    e.motion.y);
                if (!isInBounds(e.motion.x, e.motion.y))
                    wasPressed_ = false;
            }
            break;

        default:
            break;
    }
}

bool
Button::wasClicked()
{
    const auto result = wasClicked_;
    wasClicked_ = false; // Reset state now that it's been checked
    return result;
}

std::string
Button::text()
{
    return text_.text();
}

bool
Button::isInBounds(int x, int y)
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