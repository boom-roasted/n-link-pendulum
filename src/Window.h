#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

// Memory managed SDL_Window

class Window
{
public:
    explicit Window(SDL_Window* window)
        : window_(window)
    {
    }

    Window(const char* title, int x, int y, int w, int h, Uint32 flags)
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

    Window(Window&& other) noexcept
        : window_(other.window_)
    {
        other.window_ = nullptr;
    }

    Window& operator=(Window&& other) noexcept
    {
        if (&other == this)
            return *this;
        if (window_ != nullptr)
            SDL_DestroyWindow(window_);
        window_ = other.window_;
        other.window_ = nullptr;
        return *this;
    }

    // Window is not copyable
    Window(const Window& other) = delete;
    Window& operator=(const Window& other) = delete;

    ~Window() { dispose(); }

    void dispose()
    {
        if (window_ != nullptr)
        {
            SDL_DestroyWindow(window_);
            window_ = nullptr;
        }
    }

    // Cast operator for using this class with the SDL library functions
    operator SDL_Window*() { return window_; }

private:
    SDL_Window* window_;
};

#endif // WINDOW_H