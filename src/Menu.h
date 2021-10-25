#ifndef PENDULUM_MENU_H
#define PENDULUM_MENU_H

#include <vector>

#include "Button.h"
#include "Rectangle.h"
#include "Text.h"

class MainMenu
{
public:
    MainMenu(const SDL_Rect& rect, SDL_Renderer* renderer, TTF_Font* font);

    void setRect(const SDL_Rect& rect);

    void render();

    void handleEvent(SDL_Event& e, bool& shouldResume, bool& shouldQuit);

private:
    // Computes button positions
    void computePositions();

    // Button identifiers
    enum class ButtonId : int
    {
        Resume = 1,
        Options,
        Quit,
    };

    SDL_Rect rect_;
    Rectangle background_;
    SDL_Renderer* renderer_;
    std::vector<Button<Text>> buttons_;
};

#endif // PENDULUM_MENU_H
