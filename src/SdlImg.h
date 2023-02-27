#ifndef PENDULUM_SDLIMG_H
#define PENDULUM_SDLIMG_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdexcept>

#include "String.h"

class SdlImg
{
public:
    // constructor.
    SdlImg(int flags)
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading SDL IMG Library");
        if (!(IMG_Init(flags) & flags))
        {
            throw std::runtime_error(
                "SDL_image could not initialize! SDL_image Error: " +
                str(IMG_GetError()));
        }
    }

    // destructor.
    ~SdlImg()
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Quitting SDL IMG Library");
        IMG_Quit();
    }
 
    // copy constructor.
    SdlImg(const SdlImg& other) = delete;
 
    // move constructor.
    SdlImg(SdlImg&& other) noexcept = delete;
 
    // copy assignment.
    SdlImg& operator=(const SdlImg& other) = delete;
 
    // move assignment.
    SdlImg& operator=(SdlImg&& other) noexcept = delete;
private:
    
};

#endif // PENDULUM_SDLIMG_H
