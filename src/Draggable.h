#ifndef PENDULUM_DRAGGABLE_H
#define PENDULUM_DRAGGABLE_H

#include <SDL2/SDL.h>
#include <string>

#include "DragHandler.h"

template<class Content>
class Draggable
{
public:
    Draggable(const SDL_Rect& rect, int id, const Content& content)
        : id_(id)
        , content_(content)
        , dragHandler_(DragHandler(rect))
    {
    }

    SDL_Rect rect() { return content_.rect(); }

    void setRect(const SDL_Rect& rect)
    {
        content_.setRect(rect);
        dragHandler_.setRect(rect);
    }

    void render()
    {
        if (dragHandler_.isDragging())
            content_.setIsDragging();
        content_.render();
    }

    void handleEvent(SDL_Event& e) { dragHandler_.handleEvent(e); }

    bool isDragging() { return dragHandler_.isDragging(); }
    bool dragComplete() { return dragHandler_.dragComplete(); }
    SDL_Point dragStart() { return dragHandler_.dragStart(); }
    SDL_Point dragEnd() { return dragHandler_.dragEnd(); }

    int id() { return id_; }

    std::string text() { return content_.text(); }

    Content& content() { return content_; }

private:
    int id_;
    Content content_;
    DragHandler dragHandler_;
};

#endif // PENDULUM_DRAGGABLE_H
