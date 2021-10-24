#ifndef PENDULUM_BUTTON_H
#define PENDULUM_BUTTON_H

#include <SDL2/SDL.h>
#include <string>

template<class Content>
class Button
{
public:
    Button(const SDL_Rect& rect, int id, const Content& content)
        : rect_(rect)
        , id_(id)
        , content_(content)
        , isPressed_(false)
        , isClicked_(false)
    {
    }

    void setRect(const SDL_Rect& rect)
    {
        rect_ = rect;
        content_.setRect(rect);
    }

    void render()
    {
        if (isPressed_)
            content_.setIsPressed();
        content_.render();
    }

    // Tracks mouse events. Will start tracking mouse motion
    // if mouse button is down.
    void handleEvent(SDL_Event& e)
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
                    isPressed_ = true;
                }
                break;

            // When the mouse is released, it is only worthwhile when
            // the mouse was previously pressed
            case SDL_MOUSEBUTTONUP:
                if (isPressed_)
                {
                    SDL_LogDebug(
                        SDL_LOG_CATEGORY_INPUT,
                        "button %s mouse up at x=%d, y=%d",
                        text().c_str(),
                        e.button.x,
                        e.button.y);
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
                    SDL_LogDebug(
                        SDL_LOG_CATEGORY_INPUT,
                        "button %s mouse move at x=%d, y=%d",
                        text().c_str(),
                        e.motion.x,
                        e.motion.y);
                    if (!isInBounds(e.motion.x, e.motion.y))
                        isPressed_ = false;
                }
                break;

            default:
                break;
        }
    }

    // Click event only counts if the mouse if both pressed
    // AND released within the button's bounds
    bool wasClicked()
    {
        const auto result = isClicked_;
        isClicked_ = false; // Reset state now that it's been checked
        return result;
    }

    int id() { return id_; }

    std::string text() { return content_.text(); }

    Content& content() { return content_; }

private:
    bool isInBounds(int x, int y)
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

    SDL_Rect rect_;
    int id_;
    Content content_;
    bool isPressed_;
    bool isClicked_;
};

#endif // PENDULUM_BUTTON_H
