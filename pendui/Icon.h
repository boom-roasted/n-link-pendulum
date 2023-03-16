#ifndef PENDULUM_ICON_H
#define PENDULUM_ICON_H

#include <SDL2/SDL.h>
#include <string>

#include "Rectangle.h"
#include "Texture.h"

// Similar to text, but a small image
class Icon
{
public:
    Icon(
        const SDL_Rect& rect,
        const SDL_Color& bgColor,
        const std::string& iconPath,
        SDL_Renderer* renderer);

    void setRenderer(SDL_Renderer* renderer);

    SDL_Rect rect();
    void setRect(const SDL_Rect& rect);

    std::string icon();
    std::string text();
    void setIcon(const std::string& filePath);

    void setIsPressed();
    void setIsDragging();
    void darkenBackgroundNextRender();

    void render();

private:
    void loadTexture();

    SDL_Rect rect_;
    Rectangle background_;
    Texture iconTexture_;
    std::string iconPath_;
    bool darkenBackgroundNextRender_;
    SDL_Renderer* renderer_;
    bool isDirty_;
};

#endif // PENDULUM_ICON_H
