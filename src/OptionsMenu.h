#ifndef PENDULUM_OPTIONSMENU_H
#define PENDULUM_OPTIONSMENU_H

#include <vector>

#include "Button.h"
#include "PendulumOptions.h"
#include "Rectangle.h"
#include "Slider.h"

class OptionsMenu
{
public:
    OptionsMenu(const SDL_Rect& rect, const PendulumOptions& pendulumOptions, SDL_Renderer* renderer, TTF_Font* font);

    void setRect(const SDL_Rect& rect);

    void render();

    void handleEvent(SDL_Event& e);

    // State
    PendulumOptions pendulumOptions() { return pendulumOptions_; };
    bool shouldGoBack() { return shouldGoBack_; }
    bool shouldSimulate() { return shouldSimulate_; }
    void clearState();

private:
    // Computes button positions
    void computePositions();

    // Button identifiers
    enum class ButtonId : int
    {
        Back = 1,
        Simulate,
    };

    // Control identifiers
    enum class ControlId : int
    {
        NumLinks = 1,
        Mass,
    };

    SDL_Rect rect_;
    Rectangle background_;
    SDL_Renderer* renderer_;
    std::vector<Slider> controls_;
    std::vector<Button<Text>> buttons_;

    PendulumOptions pendulumOptions_;

    bool shouldGoBack_ = false;
    bool shouldSimulate_ = false;
};

#endif // PENDULUM_OPTIONSMENU_H
