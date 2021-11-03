#include "Slider.h"

#include <algorithm>

Slider::Slider(
    const SDL_Rect& rect,
    int id,
    const std::string& label,
    const Range& range,
    const double initialValue,
    const SDL_Color& bgColor,
    SDL_Renderer* renderer,
    TTF_Font* font)
    : rect_(rect)
    , id_(id)
    , background_(Rectangle(rect, bgColor))
    , bar_(Rectangle(rect, { 0, 100, 200, 255 }))
    , thumb_(Draggable<Icon>(
          rect,
          0, // Id not used yet
          Icon(rect, { 200, 200, 200, 100 }, "res/sliderThumb.png", renderer)))
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

int
Slider::id()
{
    return id_;
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
    thumb_.handleEvent(e);

    // If the thumb was dragged somewhere, update rect to stay with mouse
    if (thumb_.isDragging())
    {
        setThumbPositionFromDrag();
        snapThumbToNearestStride();
    }

    // If thumb drag finished, emit that the value changed
    if (thumb_.dragComplete())
    {
        setThumbPositionFromDrag();
        snapThumbToNearestStride();
        valueChanged_ = true;
    }
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
Slider::setThumbPositionFromDrag()
{
    const auto& r = thumb_.rect();
    const auto& barRect = bar_.rect();
    const auto newThumbX =
        std::clamp(thumb_.dragEnd().x, barRect.x, barRect.x + barRect.w);
    thumb_.setRect({ newThumbX, r.y, r.w, r.h });
}

void
Slider::snapThumbToNearestStride()
{
    const auto r = thumb_.rect();
    const auto mouseX = r.x;
    const auto strideWidth = bar_.rect().w / range_.count();

    // Start with the the far left, try each stride and see which is the closest
    int minDistanceThumbX = bar_.rect().x;
    double minDistanceValue = range_.start;
    int minDistance = std::abs(minDistanceThumbX - mouseX);
    for (int i = 1; i < range_.count() + 1; i++)
    {
        int newThumbX = bar_.rect().x + strideWidth * i;

        int distance = std::abs(newThumbX - mouseX);
        if (distance < minDistance)
        {
            minDistanceThumbX = newThumbX;
            minDistanceValue = range_.start + range_.stride * i;
            minDistance = distance;
        }
    }

    thumb_.setRect({ minDistanceThumbX, r.y, r.w, r.h });
    setValue(minDistanceValue); // TODO shouldn't duplicate computation here
}

void
Slider::computePositions()
{
    // Set up some hard coded layout parameters
    int padding = 10;
    int labelHeight = 20;
    int barHeight = 2;
    int barPadding = 10;
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