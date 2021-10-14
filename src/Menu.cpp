#include "Menu.h"

MainMenu::MainMenu(SDL_Rect rect)
    : background_(rect, SDL_Color({ 119, 181, 254, 200 }))
{
}

void
MainMenu::render(SDL_Renderer* renderer) const
{
    background_.render(renderer);
}
