#ifndef PENDULUM_SLIDER_H
#define PENDULUM_SLIDER_H

#include "Draggable.h"
#include "Icon.h"
#include "Rectangle.h"
#include "Text.h"
#include "Texture.h"
#include <SDL2/SDL.h>
#include <string>

class Slider
{
public:
    struct Range
    {
        double start;
        double stop;
        double stride;
        int count() { return (stop - start) / stride; }

        Range(double start, double stop, double stride)
            : start(start)
            , stop(stop)
            , stride(stride)
        {
        }
    };

    Slider(
        const SDL_Rect& rect,
        int id,
        const std::string& label,
        const Range& range,
        const double initialValue,
        const SDL_Color& bgColor,
        SDL_Renderer* renderer,
        TTF_Font* font);

    SDL_Rect rect();
    void setRect(const SDL_Rect& rect);

    int id();

    void render();
    void handleEvent(SDL_Event& e);

    double value();
    void setValue(double newValue);
    bool valueChanged();

private:
    // Move thumb to the nearest viable position
    // Only move along the x axis. Because that's how the slider works.
    // Clamps to bar dimensions so that thumb stays within bar bounds.
    void snapThumbToNearestStride(int targetX);

    int computeThumbOffset();

    void computePositions();

    std::string toString(double val);

    SDL_Rect rect_;
    int id_;
    Rectangle background_;
    Rectangle bar_;
    Draggable<Icon> thumb_;
    Text label_;
    Text displayValue_;
    Range range_;
    SDL_Renderer* renderer_;
    double value_;
    bool valueChanged_;
};

#endif // PENDULUM_SLIDER_H
