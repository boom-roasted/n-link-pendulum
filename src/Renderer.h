#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>

// Memory managed SDL_Renderer

class Renderer
{
public:
    explicit Renderer(SDL_Renderer* renderer);

    Renderer(SDL_Window* window, int index, Uint32 flags);

    Renderer(Renderer&& other) noexcept;

    Renderer& operator=(Renderer&& other) noexcept;

    // Renderer is not copyable
    Renderer(const Renderer& other) = delete;
    Renderer& operator=(const Renderer& other) = delete;

    ~Renderer();

    void dispose();

    // Cast opertator. Converts the C++ wrapper object into C object
    // when it is used with the C functions that come with the library
    operator SDL_Renderer*() { return renderer_; }

private:
    SDL_Renderer* renderer_;
};

#endif // RENDERER_H