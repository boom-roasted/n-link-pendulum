#include "Window.h"

Window::Window(SDL_Window* window)
    : window_(window)
{
}

Window::Window(const char* title, int x, int y, int w, int h, Uint32 flags)
{
    window_ = SDL_CreateWindow(title, x, y, w, h, flags);
    if (window_ == nullptr)
    {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_VIDEO,
            "Window could not be created! SDL Error: %s\n",
            SDL_GetError());
        SDL_assert(window_ != nullptr);
    }
}

Window::Window(Window&& other) noexcept
    : window_(other.window_)
{
    other.window_ = nullptr;
}

Window&
Window::operator=(Window&& other) noexcept
{
    if (&other == this)
        return *this;
    if (window_ != nullptr)
        SDL_DestroyWindow(window_);
    window_ = other.window_;
    other.window_ = nullptr;
    return *this;
}

Window::~Window()
{
    dispose();
}

void
Window::dispose()
{
    if (window_ != nullptr)
    {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
}
