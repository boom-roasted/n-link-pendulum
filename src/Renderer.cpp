#include "Renderer.h"

Renderer::Renderer(SDL_Renderer* renderer)
    : renderer_(renderer)
{
}

Renderer::Renderer(SDL_Window* window, int index, Uint32 flags)
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

Renderer::Renderer(Renderer&& other) noexcept
    : renderer_(other.renderer_)
{
    other.renderer_ = nullptr;
}

Renderer&
Renderer::operator=(Renderer&& other) noexcept
{
    if (&other == this)
        return *this;
    if (renderer_ != nullptr)
        SDL_DestroyRenderer(renderer_);
    renderer_ = other.renderer_;
    other.renderer_ = nullptr;
    return *this;
}

Renderer::~Renderer()
{
    if (renderer_)
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Destroying renderer");
        SDL_DestroyRenderer(renderer_);
    }
}
