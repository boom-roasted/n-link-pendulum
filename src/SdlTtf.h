#ifndef PENDULUM_SDLTTF_H
#define PENDULUM_SDLTTF_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdexcept>

#include "String.h"

class SdlTtf
{
public:
    // constructor.
    SdlTtf()
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading SDL TTF Library");
        if (TTF_Init() == -1)
        {
            throw std::runtime_error(
                "SDL_ttf could not initialize! SDL_ttf Error: " +
                str(TTF_GetError()));
        }
    }

    // destructor.
    ~SdlTtf()
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Quitting SDL TTF Library");
        TTF_Quit();
    }
 
    // copy constructor.
    SdlTtf(const SdlTtf& other) = delete;
 
    // move constructor.
    SdlTtf(SdlTtf&& other) noexcept = delete;
 
    // copy assignment.
    SdlTtf& operator=(const SdlTtf& other) = delete;
 
    // move assignment.
    SdlTtf& operator=(SdlTtf&& other) noexcept = delete;
private:
    
};

#endif // PENDULUM_SDLTTF_H
