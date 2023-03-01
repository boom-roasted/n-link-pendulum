#ifndef PENDULUM_MENU_H
#define PENDULUM_MENU_H

#include <optional>
#include <vector>

#include "Button.h"
#include "OptionsMenu.h"
#include "Rectangle.h"
#include "Text.h"

class MainMenu
{
public:
    MainMenu(
        const SDL_Rect& rect,
        const PendulumOptions& pendulumOptions,
        SDL_Renderer* renderer,
        TTF_Font* font);

    void setRect(const SDL_Rect& rect);

    void render();

    void handleEvent(SDL_Event& e);

    PendulumOptions pendulumOptions() { return pendulumOptions_; }
    bool shouldResume() { return shouldResume_; }
    bool shouldQuit() { return shouldQuit_; }
    bool shouldSimulate() { return shouldSimulate_; }
    void clearState();

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
    TTF_Font* font_;
    std::vector<Button<Text>> buttons_;
    std::optional<OptionsMenu> optionsMenu_;

    PendulumOptions pendulumOptions_;

    bool shouldResume_ = false;
    bool shouldQuit_ = false;
    bool shouldSimulate_ = false;
};

#endif // PENDULUM_MENU_H
