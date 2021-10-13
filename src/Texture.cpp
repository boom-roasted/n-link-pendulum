#include "Texture.h"

Texture::Texture()
{
    // Initialize
    texture_ = NULL;
    width_ = 0;
    height_ = 0;
}

Texture::~Texture()
{
    // Deallocate
    free();
}

bool
Texture::loadFromFile(const std::string& path, SDL_Renderer* renderer)
{
    // Get rid of preexisting texture
    free();

    // The final texture
    SDL_Texture* newTexture = NULL;

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf(
            "Unable to load image %s! SDL_image Error: %s\n",
            path.c_str(),
            IMG_GetError());
    }
    else
    {
        // Color key image
        SDL_SetColorKey(
            loadedSurface,
            SDL_TRUE,
            SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf(
                "Unable to create texture from %s! SDL Error: %s\n",
                path.c_str(),
                SDL_GetError());
        }
        else
        {
            // Get image dimensions
            width_ = loadedSurface->w;
            height_ = loadedSurface->h;
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    // Return success
    texture_ = newTexture;
    return texture_ != NULL;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool
Texture::loadFromRenderedText(
    const std::string& textureText,
    SDL_Color textColor)
{
    // Get rid of preexisting texture
    free();

    // Render text surface
    SDL_Surface* textSurface =
        TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
    if (textSurface != NULL)
    {
        // Create texture from surface pixels
        texture_ = SDL_CreateTextureFromSurface(renderer_, textSurface);
        if (texture_ == NULL)
        {
            printf(
                "Unable to create texture from rendered text! SDL Error: %s\n",
                SDL_GetError());
        }
        else
        {
            // Get image dimensions
            width_ = textSurface->w;
            height_ = textSurface->h;
        }

        // Get rid of old surface
        SDL_FreeSurface(textSurface);
    }
    else
    {
        printf(
            "Unable to render text surface! SDL_ttf Error: %s\n",
            TTF_GetError());
    }

    // Return success
    return texture_ != NULL;
}
#endif

void
Texture::free()
{
    // Free texture if it exists
    if (texture_ != NULL)
    {
        SDL_DestroyTexture(texture_);
        texture_ = NULL;
        width_ = 0;
        height_ = 0;
    }
}

void
Texture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
    // Modulate texture rgb
    SDL_SetTextureColorMod(texture_, red, green, blue);
}

void
Texture::setBlendMode(SDL_BlendMode blending)
{
    // Set blending function
    SDL_SetTextureBlendMode(texture_, blending);
}

void
Texture::setAlpha(Uint8 alpha)
{
    // Modulate texture alpha
    SDL_SetTextureAlphaMod(texture_, alpha);
}

void
Texture::render(
    int x,
    int y,
    SDL_Renderer* renderer,
    bool shouldCenter,
    SDL_Rect* clip,
    double angle,
    SDL_Point* center,
    SDL_RendererFlip flip)
{
    // Set rendering space and render to screen
    const int topLeftX = shouldCenter ? x - 0.5 * width_ : x;
    const int topLeftY = shouldCenter ? y - 0.5 * height_ : y;
    SDL_Rect renderQuad = { topLeftX, topLeftY, width_, height_ };

    // Set clip rendering dimensions
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    // Render to screen
    SDL_RenderCopyEx(
        renderer, texture_, clip, &renderQuad, angle, center, flip);
}

int
Texture::getWidth()
{
    return width_;
}

int
Texture::getHeight()
{
    return height_;
}