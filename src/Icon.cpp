#include "Icon.h"

Icon::Icon(
    const SDL_Rect& rect,
    const SDL_Color& bgColor,
    const std::string& iconPath,
    SDL_Renderer* renderer)
    : rect_(rect)
    , background_(rect, bgColor)
    , iconTexture_(Texture())
    , iconPath_(iconPath)
    , darkenBackgroundNextRender_(false)
    , renderer_(renderer)
{
    setIcon(iconPath);
}

void
Icon::setRect(const SDL_Rect& rect)
{
    rect_ = rect;
    background_.setRect(rect);
}

void
Icon::setIcon(const std::string& filePath)
{
    if (!iconTexture_.loadFromFile(filePath, renderer_))
    {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_RENDER, "Unable to load Icon texture!\n");
    }
}

void
Icon::darkenBackgroundNextRender()
{
    darkenBackgroundNextRender_ = true;
}

void
Icon::render()
{
    // Darken background if requested
    SDL_Color bg;

    if (darkenBackgroundNextRender_)
    {
        bg = background_.color(); // TODO darken
        darkenBackgroundNextRender_ = false;
    }
    else
    {
        bg = background_.color();
    }

    // Render the background
    background_.render(renderer_, bg);

    // Render the image, aligned to the center of this element
    int x = rect_.x + (0.5 * rect_.w) - (0.5 * iconTexture_.getWidth());
    int y = rect_.y + (0.5 * rect_.h) - (0.5 * iconTexture_.getHeight());
    iconTexture_.render(x, y, renderer_);
}