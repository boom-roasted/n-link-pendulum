#include "Text.h"

Text::Text(
    const SDL_Rect& rect,
    const SDL_Color& fgColor,
    const SDL_Color& bgColor,
    const std::string& text,
    SDL_Renderer* renderer,
    TTF_Font* font)
    : rect_(rect)
    , background_(rect, bgColor)
    , fgColor_(fgColor)
    , textTexture_(Texture())
    , text_(text)
    , renderer_(renderer)
    , font_(font)
    , isPressed_(false)
    , isDirty_(true)
{
}

void
Text::setRenderer(SDL_Renderer* renderer)
{
    renderer_ = renderer;
    isDirty_ = true;
}

void
Text::setRect(const SDL_Rect& rect)
{
    rect_ = rect;
    background_.setRect(rect);
}

void
Text::setText(const std::string& text)
{
    text_ = text;
    isDirty_ = true;
}

void
Text::loadTexture()
{
    // Render the text in the foreground color
    SDL_assert(textTexture_.loadFromRenderedText(
        renderer_, text_.c_str(), font_, fgColor_));
}

void
Text::setIsPressed()
{
    isPressed_ = true;
    swapFgBg();
}

void
Text::swapFgBg()
{
    const auto fg = fgColor_;
    const auto bg = background_.color();
    fgColor_ = bg;
    background_.setColor(fg);

    // Need to update loaded texture to show current colors
    isDirty_ = true;
}

void
Text::render()
{
    // Render the background
    background_.render(renderer_);

    // Re-load text if needed
    if (isDirty_)
    {
        loadTexture();
        isDirty_ = false;
    }

    // Render text, aligned to the center of this element
    int x = rect_.x + (0.5 * rect_.w) - (0.5 * textTexture_.getWidth());
    int y = rect_.y + (0.5 * rect_.h) - (0.5 * textTexture_.getHeight());
    textTexture_.render(x, y, renderer_);

    // Swap foreground and background colors back if necessary
    if (isPressed_)
    {
        swapFgBg();
        isPressed_ = false;
    }
}

std::string
Text::text()
{
    return text_;
}
