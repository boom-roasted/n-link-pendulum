#ifndef MENU_H
#define MENU_H

#include <vector>

#include "Rectangle.h"
#include "Text.h"

class MainMenu
{
public:
    MainMenu(SDL_Rect rect);

    void render(SDL_Renderer* renderer, TTF_Font* font);

private:
    Rectangle background_;
    std::vector<Text> texts_;
};

#endif // MENU_H
