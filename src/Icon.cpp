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
    , isDirty_(true)
{
}

SDL_Rect
Icon::rect()
{
    return rect_;
}

void
Icon::setRect(const SDL_Rect& rect)
{
    rect_ = rect;
    background_.setRect(rect);
}

std::string
Icon::icon()
{
    return iconPath_;
}

std::string
Icon::text()
{
    return icon();
}

void
Icon::setIcon(const std::string& filePath)
{
    if (iconPath_ == filePath)
        return;
    iconPath_ = filePath;
    isDirty_ = true;
}

void
Icon::loadTexture()
{
    if (!iconTexture_.loadFromFile(iconPath_, renderer_))
    {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_RENDER, "Unable to load Icon texture!\n");
    }
}

void
Icon::setIsPressed()
{
    darkenBackgroundNextRender();
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
    SDL_Color bg = background_.color();

    if (darkenBackgroundNextRender_)
    {
        bg.r = 255; // TODO darken
        darkenBackgroundNextRender_ = false;
    }

    // Render the background
    background_.render(renderer_, bg);

    // Re-load the texture if it has changed
    if (isDirty_)
    {
        loadTexture();
        isDirty_ = false;
    }

    // Render the image, aligned to the center of this element
    int x = rect_.x + (0.5 * rect_.w) - (0.5 * iconTexture_.getWidth());
    int y = rect_.y + (0.5 * rect_.h) - (0.5 * iconTexture_.getHeight());
    iconTexture_.render(x, y, renderer_);
}