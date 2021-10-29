#ifndef PENDULUM_BUTTON_H
#define PENDULUM_BUTTON_H

#include <SDL2/SDL.h>
#include <string>

#include "Clickable.h"

template<class Content>
class Button
{
public:
    Button(const SDL_Rect& rect, int id, const Content& content)
        : id_(id)
        , content_(content)
        , clickable_(Clickable(rect))
    {
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
