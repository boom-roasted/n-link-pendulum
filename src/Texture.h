#ifndef PENDULUM_TEXTURE_H
#define PENDULUM_TEXTURE_H

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class Texture
{
public:
    Texture();

    // Deallocates memory
    ~Texture();

    // Loads image at specified path
    bool loadFromFile(const std::string& path, SDL_Renderer* renderer);

#if defined(SDL_TTF_MAJOR_VERSION)
    // Creates image from font string
    bool loadFromRenderedText(
        SDL_Renderer* renderer,
        const std::string& textureText,
        TTF_Font* font,
        SDL_Color textColor);
#endif

    // Deallocates texture
    void free();

    // Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    // Set blending
    void setBlendMode(SDL_BlendMode blending);

    // Set alpha modulation
    void setAlpha(Uint8 alpha);

    // Renders texture at given point
    void render(
        int x,
        int y,
        SDL_Renderer* renderer,
        bool shouldCenter = false,
        SDL_Rect* clip = NULL,
        double angle = 0.0,
        SDL_Point* center = NULL,
        SDL_RendererFlip flip = SDL_FLIP_NONE);

    // Gets image dimensions
    int getWidth();
    int getHeight();

private:
    // The actual hardware texture
    SDL_Texture* texture_;

    // Image dimensions
    int width_;
    int height_;
};

#endif // PENDULUM_TEXTURE_H
