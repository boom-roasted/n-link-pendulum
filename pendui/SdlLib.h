#ifndef PENDULUM_SDLLIB_H
#define PENDULUM_SDLLIB_H

#include <SDL2/SDL.h>
#include <stdexcept>

#include "String.h"

// https://en.cppreference.com/w/cpp/language/rule_of_three
class SdlLib
{
public:
    // constructor.
    SdlLib()
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading SDL Library");
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            throw std::runtime_error("SDL could not initialize! SDL Error: " + str(SDL_GetError()));
        }

        // Set texture filtering to linear.
        // Not unrecoverable to fail?
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            SDL_LogWarn(
                SDL_LOG_CATEGORY_VIDEO,
                "Warning: Linear texture filtering not enabled!");
        }
    }

    // destructor.
    ~SdlLib()
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Quitting SDL Library");
        SDL_Quit();
    }
 
    // copy constructor.
    SdlLib(const SdlLib& other) = delete;
 
    // move constructor.
    SdlLib(SdlLib&& other) noexcept = delete;
 
    // copy assignment.
    SdlLib& operator=(const SdlLib& other) = delete;
 
    // move assignment.
    SdlLib& operator=(SdlLib&& other) noexcept = delete;
private:
    
};

#endif // PENDULUM_SDLLIB_H
