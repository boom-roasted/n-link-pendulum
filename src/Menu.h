#ifndef MENU_H
#define MENU_H

#include <vector>

#include "Button.h"
#include "Rectangle.h"

class MainMenu
{
public:
    MainMenu(SDL_Rect rect);

    void render(SDL_Renderer* renderer, TTF_Font* font);

    void handleEvent(SDL_Event& e);

private:
    Rectangle background_;
    std::vector<Button> buttons_;
};

#endif // MENU_H
