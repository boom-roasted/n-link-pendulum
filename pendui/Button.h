#ifndef PENDULUM_BUTTON_H
#define PENDULUM_BUTTON_H

#include <SDL2/SDL.h>
#include <string>

#include "Clickable.h"
#include "Hotkey.h"

template<class Content>
class Button
{
    Button(
        const SDL_Rect& rect,
        int id,
        const Content& content,
        const Clickable& clickable)
        : id_(id)
        , content_(content)
        , clickable_(clickable)
    {
    }

public:
    // clang-format off
    Button(const SDL_Rect& rect, int id, const Content& content)
        : Button(rect, id, content, Clickable(rect))
    {
    }

    Button(
        const SDL_Rect& rect, int id, const Content& content, const Hotkey& hotkey)
        : Button(rect, id, content, Clickable(rect, hotkey))
    {
    }

    Button(const SDL_Rect& rect, int id, const Content& content, const std::vector<Hotkey>& hotkeys)
        : Button(rect, id, content, Clickable(rect, hotkeys))
    {
    }
    // clang-format on

    void
    setRenderer(SDL_Renderer* renderer)
    {
        content_.setRenderer(renderer);
    }

    void setRect(const SDL_Rect& rect)
    {
        content_.setRect(rect);
        clickable_.setRect(rect);
    }

    void render()
    {
        // Intentionally not rendering underlying view since the content would
        // obscure it
        if (clickable_.isPressed())
            content_.setIsPressed();
        content_.render();
    }

    void handleEvent(SDL_Event& e) { clickable_.handleEvent(e); }

    bool wasClicked() { return clickable_.wasClicked(); }

    int id() { return id_; }

    std::string text() { return content_.text(); }

    Content& content() { return content_; }

private:
    int id_;
    Content content_;
    Clickable clickable_;
};

#endif // PENDULUM_BUTTON_H
