#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>

// Memory managed SDL_Renderer

class Renderer
{
public:
    explicit Renderer(SDL_Renderer* renderer)
        : renderer_(renderer)
    {
    }

    Renderer(SDL_Window* window, int index, Uint32 flags)
    {
        renderer_ = SDL_CreateRenderer(window, index, flags);
        if (renderer_ == nullptr)
        {
            SDL_LogCritical(
                SDL_LOG_CATEGORY_VIDEO,
                "Renderer could not be created! SDL Error: %s\n",
                SDL_GetError());
            SDL_assert(renderer_ != nullptr);
        }
    }

    Renderer(Renderer&& other) noexcept
        : renderer_(other.renderer_)
    {
        other.renderer_ = nullptr;
    }

    Renderer& operator=(Renderer&& other) noexcept
    {
        if (&other == this)
            return *this;
        if (renderer_ != nullptr)
            SDL_DestroyRenderer(renderer_);
        renderer_ = other.renderer_;
        other.renderer_ = nullptr;
        return *this;
    }

    // Renderer is not copyable
    Renderer(const Renderer& other) = delete;
    Renderer& operator=(const Renderer& other) = delete;

    ~Renderer() { dispose(); }

    void dispose()
    {
        if (renderer_ != nullptr)
        {
            SDL_DestroyRenderer(renderer_);
            renderer_ = nullptr;
        }
    }

    // Cast opertator. Converts the C++ wrapper object into C object
    // when it is used with the C functions that come with the library
    operator SDL_Renderer*() { return renderer_; }

private:
    SDL_Renderer* renderer_;
};

#endif // RENDERER_H