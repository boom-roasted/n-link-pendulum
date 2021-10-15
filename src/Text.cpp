#include "Text.h"

Text::Text(
    const SDL_Rect& rect,
    const SDL_Color& fgColor,
    const SDL_Color& bgColor,
    const std::string& text)
    : rect_(rect)
    , background_(rect, bgColor)
    , fgColor_(fgColor)
    , textTexture_(Texture())
    , text_(text)
    , swapFgBgNextRender_(false)
{
}

void
Text::swapFgBgNextRender()
{
    swapFgBgNextRender_ = true;
}

void
Text::render(SDL_Renderer* renderer, TTF_Font* font)
{
    // Set foreground and background depending on color swap
    SDL_Color fg;
    SDL_Color bg;

    if (swapFgBgNextRender_)
    {
        fg = background_.color();
        bg = fgColor_;
        swapFgBgNextRender_ = false;
    }
    else
    {
        fg = fgColor_;
        bg = background_.color();
    }

    // Render the background
    background_.render(renderer, bg);

    // Render the text in the foreground
    // TODO would it be better to only have this loaded once?
    if (!textTexture_.loadFromRenderedText(renderer, text_.c_str(), font, fg))
    {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_RENDER, "Unable to render Text texture!\n");
    }

    // Render fps text, aligned to the center of this element
    int x = rect_.x + (0.5 * rect_.w) - (0.5 * textTexture_.getWidth());
    int y = rect_.y + (0.5 * rect_.h) - (0.5 * textTexture_.getHeight());
    textTexture_.render(x, y, renderer);
}

std::string
Text::text()
{
    return text_;
}