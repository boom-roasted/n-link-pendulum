#ifndef PENDULUM_DRAGHANDLER_H
#define PENDULUM_DRAGHANDLER_H

#include <SDL2/SDL.h>

class DragHandler
{
public:
    DragHandler(const SDL_Rect& rect);

    void setRect(const SDL_Rect& rect);

    // Tracks mouse events. Will start tracking mouse motion
    // if mouse button is down.
    void handleEvent(SDL_Event& e);

    bool isDragging();
    bool dragComplete();
    SDL_Point dragStart();
    SDL_Point dragEnd();

private:
    SDL_Rect rect_;
    bool isDragging_;
    bool dragComplete_;
    SDL_Point dragStart_;
    SDL_Point dragEnd_;
};

#endif // PENDULUM_DRAGHANDLER_H
