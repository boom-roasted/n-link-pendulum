#ifndef PENDULUM_WINDOW_H
#define PENDULUM_WINDOW_H

#include <SDL2/SDL.h>

// Memory managed SDL_Window

class Window
{
public:
    explicit Window(SDL_Window* window);
    
    Window(const char* title, int x, int y, int w, int h, Uint32 flags);
    Window(Window&& other) noexcept;

    Window& operator=(Window&& other) noexcept;

    // Window is not copyable
    Window(const Window& other) = delete;
    Window& operator=(const Window& other) = delete;

    ~Window();

    void dispose();

    // Cast operator for using this class with the SDL library functions
    operator SDL_Window*() { return window_; }

private:
    SDL_Window* window_;
};

#endif // PENDULUM_WINDOW_H
