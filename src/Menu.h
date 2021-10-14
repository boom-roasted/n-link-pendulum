#ifndef MENU_H
#define MENU_H

#include "Rectangle.h"

class MainMenu
{
public:
    MainMenu(SDL_Rect rect);

    void render(SDL_Renderer* renderer) const;

private:
    Rectangle background_;
};

#endif // MENU_H
