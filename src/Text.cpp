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
{
}

void
Text::render(SDL_Renderer* renderer, TTF_Font* font)
{
    // Render the background
    background_.render(renderer);

    // Render the text in the foreground
    // TODO would it be better to only have this loaded once?
    if (!textTexture_.loadFromRenderedText(
            renderer, text_.c_str(), font, fgColor_))
    {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_RENDER, "Unable to render Text texture!\n");
    }

    // Render fps text, aligned to the center of this element
    int x = rect_.x + (0.5 * rect_.w) - (0.5 * textTexture_.getWidth());
    int y = rect_.y + (0.5 * rect_.h) - (0.5 * textTexture_.getHeight());
    textTexture_.render(x, y, renderer);
}
