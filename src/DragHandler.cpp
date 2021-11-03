#include "DragHandler.h"

DragHandler::DragHandler(const SDL_Rect& rect)
    : rect_(rect)
    , isDragging_(false)
    , dragComplete_(false)
    , dragStart_({ 0, 0 })
    , dragEnd_({ 0, 0 })
{
}

void
DragHandler::setRect(const SDL_Rect& rect)
{
    rect_ = rect;
}

void
DragHandler::handleEvent(SDL_Event& e)
{
    ;
    switch (e.type)
    {
        // When the mouse is down, note that
        case SDL_MOUSEBUTTONDOWN:
        {
            SDL_Point p = { e.button.x, e.button.y };
            if (SDL_PointInRect(&p, &rect_))
            {
                isDragging_ = true;
                dragStart_ = p;
                dragEnd_ = p;
            }
            break;
        }

        // When the mouse is released, it is only worthwhile when
        // the mouse was previously pressed
        case SDL_MOUSEBUTTONUP:
            if (isDragging_)
            {
                isDragging_ = false;
                dragComplete_ = true;
            }
            break;

        // When the mouse is down, we need to follow it around.
        case SDL_MOUSEMOTION:
            if (isDragging_)
            {
                dragEnd_ = { e.motion.x, e.motion.y };
            }
            break;

        default:
            break;
    }
}

bool
DragHandler::isDragging()
{
    return isDragging_;
}
bool
DragHandler::dragComplete()
{
    auto result = dragComplete_;
    dragComplete_ = false;
    return result;
}

SDL_Point
DragHandler::dragStart()
{
    return dragStart_;
}

SDL_Point
DragHandler::dragEnd()
{
    return dragEnd_;
}
