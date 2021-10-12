#ifndef DOT_H
#define DOT_h

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
    Dot(int boundingWidth, int boundingHeight);

    // Takes key presses and adjusts the dot's velocity
    void handleEvent(SDL_Event& e);

    // Moves the dot
    void move();

    // Shows the dot on the screen
    void render(Texture& texture, SDL_Renderer* renderer);

private:
    // The X and Y offsets of the dot
    int posX_, posY_;

    // The velocity of the dot
    int velX_, velY_;

    // Bounds of the dot
    int boundingWidth_;
    int boundingHeight_;
};

#endif // DOT_H