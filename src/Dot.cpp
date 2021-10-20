#include "Dot.h"

#include <iostream>

Dot::Dot(int boundingWidth, int boundingHeight, SDL_Renderer* renderer)
{
    // Set bounds
    boundingWidth_ = boundingWidth;
    boundingHeight_ = boundingHeight;

    // Set renderer
    renderer_ = renderer;

    // Load dot texture
    SDL_assert(texture_.loadFromFile("res/dot.bmp", renderer_));

    // Initialize the offsets
    posX_ = 0;
    posY_ = 0;

    // Initialize the velocity
    velX_ = 0;
    velY_ = 0;
}

void
Dot::handleEvent(SDL_Event& e)
{
    // If a key was pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        // Adjust the velocity
        switch (e.key.keysym.sym)
        {
            case SDLK_UP:
                velY_ -= DOT_VEL;
                break;
            case SDLK_DOWN:
                velY_ += DOT_VEL;
                break;
            case SDLK_LEFT:
                velX_ -= DOT_VEL;
                break;
            case SDLK_RIGHT:
                velX_ += DOT_VEL;
                break;
        }
    }
    // If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        // Adjust the velocity
        switch (e.key.keysym.sym)
        {
            case SDLK_UP:
                velY_ += DOT_VEL;
                break;
            case SDLK_DOWN:
                velY_ -= DOT_VEL;
                break;
            case SDLK_LEFT:
                velX_ += DOT_VEL;
                break;
            case SDLK_RIGHT:
                velX_ -= DOT_VEL;
                break;
        }
    }
}

void
Dot::move()
{
    // Move the dot left or right
    posX_ += velX_;

    // If the dot went too far to the left or right
    if ((posX_ < 0) || (posX_ + DOT_WIDTH > boundingWidth_))
    {
        // Move back
        posX_ -= velX_;
    }

    // Move the dot up or down
    posY_ += velY_;

    // If the dot went too far up or down
    if ((posY_ < 0) || (posY_ + DOT_HEIGHT > boundingHeight_))
    {
        // Move back
        posY_ -= velY_;
    }
}

void
Dot::setBounds(int w, int h)
{
    boundingWidth_ = w;
    boundingHeight_ = h;

    // Fit within new width
    if (posX_ + DOT_WIDTH > boundingWidth_)
        posX_ = boundingWidth_ - DOT_WIDTH;

    // Fit within new height
    if (posY_ + DOT_HEIGHT > boundingHeight_)
        posY_ = boundingHeight_ - DOT_HEIGHT;
}

void
Dot::render()
{
    // Show the dot
    texture_.render(posX_, posY_, renderer_);
}
