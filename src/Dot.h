#ifndef DOT_H
#define DOT_H

#include <SDL2/SDL.h>

#include "Texture.h"

// The dot that will move around on the screen
class Dot
{
public:
    // The dimensions of the dot
    static const int DOT_WIDTH = 20;
    static const int DOT_HEIGHT = 20;

    // Maximum axis velocity of the dot
    static const int DOT_VEL = 10;

    // Initializes the variables
    Dot(int boundingWidth, int boundingHeight, SDL_Renderer* renderer);

    // Takes key presses and adjusts the dot's velocity
    void handleEvent(SDL_Event& e);

    // Moves the dot
    void move();

    // (Re)set the bounds of the dot
    // If reducing in size, dot is moved to fit within new bounds
    void setBounds(int w, int h);

    // Shows the dot on the screen
    void render();

private:
    // The X and Y offsets of the dot
    int posX_, posY_;

    // The velocity of the dot
    int velX_, velY_;

    // Bounds of the dot
    int boundingWidth_;
    int boundingHeight_;

    // Reference to renderer
    SDL_Renderer* renderer_;

    // Dot texture
    Texture texture_;
};

#endif // DOT_H