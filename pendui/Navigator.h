#ifndef PENDULUM_NAVIGATOR_H
#define PENDULUM_NAVIGATOR_H

#include <SDL2/SDL.h>
#include <vector>

#include "Button.h"
#include "Icon.h"

class Navigator
{
public:
    Navigator(const SDL_Rect& rect, SDL_Renderer* renderer);

    void setRenderer(SDL_Renderer* renderer);
    void setRect(const SDL_Rect& rect);

    // Navigator state
    bool shouldZoomIn() { return shouldZoomIn_; }
    bool shouldZoomOut() { return shouldZoomOut_; }
    bool shouldZoomFit() { return shouldZoomFit_; }
    void clearState(); // Resets the 'should*' flags

    void render();

    // Button identifiers
    enum class ButtonId : int
    {
        ZoomIn = 1,
        ZoomOut,
        ZoomFit,
    };

    // Handles event
    void handleEvent(SDL_Event& e);

private:
    // Computes button positions
    void computePositions();

    SDL_Rect rect_;
    Rectangle background_;
    std::vector<Button<Icon>> buttons_;
    SDL_Renderer* renderer_;

    bool shouldZoomIn_ = false;
    bool shouldZoomOut_ = false;
    bool shouldZoomFit_ = false;
};

#endif // PENDULUM_NAVIGATOR_H
