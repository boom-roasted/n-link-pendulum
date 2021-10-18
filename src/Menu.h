#ifndef MENU_H
#define MENU_H

#include <vector>

#include "Button.h"
#include "Rectangle.h"

class MainMenu
{
public:
    MainMenu(const SDL_Rect& rect);

    void setRect(const SDL_Rect& rect);

    void render(SDL_Renderer* renderer, TTF_Font* font);

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

    struct ButtonData
    {
        ButtonId id;
        std::string name;

        ButtonData(ButtonId id, const std::string& name)
            : id(id)
            , name(name)
        {
        }
    };

    SDL_Rect rect_;
    Rectangle background_;
    std::vector<Button> buttons_;
};

#endif // MENU_H
