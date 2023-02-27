#ifndef PENDULUM_FONT_H
#define PENDULUM_FONT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdexcept>

#include "String.h"

class Font
{
public:
    // constructor.
    Font(std::string file, int ptsize)
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading font: %s with size %d", file.c_str(), ptsize);
        font_ = TTF_OpenFont(file.c_str(), ptsize);
        if (font_ == nullptr)
        {
            throw std::runtime_error("Failed to load ttf font! SDL_ttf Error: " + str(TTF_GetError()));
        }
    }

    // destructor.
    ~Font()
    {
        if (font_)
        {
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Closing font: %s", TTF_FontFaceFamilyName(font_));
            TTF_CloseFont(font_);
        }
    }
 
    // copy constructor.
    Font(const Font& other) = delete;
 
    // move constructor.
    Font(Font&& other) noexcept = delete;
 
    // copy assignment.
    Font& operator=(const Font& other) = delete;
 
    // move assignment.
    Font& operator=(Font&& other) noexcept = delete;

    // Cast opertator. Converts the C++ wrapper object into C object
    // when it is used with the C functions that come with the library
    operator TTF_Font*() { return font_; }
private:
    TTF_Font* font_ = nullptr;
};

#endif // PENDULUM_FONT_H
