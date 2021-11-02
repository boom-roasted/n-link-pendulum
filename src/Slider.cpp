#include "Slider.h"

Slider::Slider(
    const SDL_Rect& rect,
    const std::string& label,
    const Range& range,
    const double initialValue,
    const SDL_Color& bgColor,
    SDL_Renderer* renderer,
    TTF_Font* font)
    : rect_(rect)
    , background_(Rectangle(rect, bgColor))
    , bar_(Rectangle(rect, { 0, 100, 200, 255 }))
    , thumb_(
          Icon(rect, { 200, 200, 200, 100 }, "res/sliderThumb.png", renderer))
    , label_(Text(rect, { 0, 0, 0, 255 }, bgColor, label, renderer, font))
    , displayValue_(Text(rect, { 0, 0, 0, 255 }, bgColor, " ", renderer, font))
    , range_(range)
    , renderer_(renderer)
    , value_(initialValue)
    , valueChanged_(false)
{
    setValue(initialValue);
}

SDL_Rect
Slider::rect()
{
    return rect_;
}

void
Slider::setRect(const SDL_Rect& rect)
{
    rect_ = rect;
    background_.setRect(rect);
    computePositions();
}

void
Slider::render()
{
    // Render background
    background_.render(renderer_);

    // Render label
    label_.render();

    // Render value
    displayValue_.render();

    // Render slider bar
    bar_.render(renderer_);

    // Render slider thumb
    thumb_.render();
}

void
Slider::handleEvent(SDL_Event& e)
{
}

double
Slider::value()
{
    return value_;
}

void
Slider::setValue(double newValue)
{
    displayValue_.setText(toString(newValue));
    value_ = newValue;
    const auto barRect = bar_.rect();
    const auto thumbRect = thumb_.rect();
    const auto thumbOffset = computeThumbOffset(barRect.w);
    thumb_.setRect(
        { barRect.x + thumbOffset, thumbRect.y, thumbRect.w, thumbRect.h });
}

bool
Slider::valueChanged()
{
    const auto result = valueChanged_;
    valueChanged_ = false;
    return result;
}

int
Slider::computeThumbOffset(int barWidth)
{
    if (value_ < range_.start) // OOB low
        return 0;
    else if (value_ > range_.stop) // OOB high
        return barWidth;
    else
    {
        const auto strideWidth = barWidth / range_.count();
        const int numStrides = (value_ - range_.start) / range_.stride;
        return strideWidth * numStrides;
    }
}

void
Slider::computePositions()
{
    // Set up some hard coded layout parameters
    int padding = 10;
    int labelHeight = 20;
    int barHeight = 10;
    int barPadding = 20;
    int valueWidth = 60;
    int thumbSize = 20;

    // Compute some helpful intermediaries
    int x = rect_.x + padding;
    int y = rect_.y + padding;
    int w = rect_.w - padding - padding;
    // int h = rect_.h - padding - padding;

    int barWidth = w - valueWidth;

    // The label goes first
    label_.setRect({ x, y, w, labelHeight });

    // Next is the slider bar
    bar_.setRect({
        x,
        y + labelHeight + barPadding,
        barWidth,
        barHeight,
    });

    // Now the selected value
    displayValue_.setRect({
        x + barWidth,
        y + labelHeight + barPadding,
        valueWidth,
        barHeight,
    });

    // Now the slider thumb
    thumb_.setRect({
        x + computeThumbOffset(barWidth),
        y + labelHeight + barPadding,
        thumbSize,
        thumbSize,
    });
}

std::string
Slider::toString(double val)
{
    const auto s = std::to_string(val);
    std::size_t found = s.find(".");
    if (found == std::string::npos)
        return s;
    if (found >= s.size() - 3)
        return s;
    const auto sub = s.substr(0, found + 3);
    if (s[found + 1] == '0' && s[found + 2] == '0')
        return sub.substr(0, found);
    return sub;
}