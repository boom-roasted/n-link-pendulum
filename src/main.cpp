#include <fstream>
#include <stdio.h>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "MainWindow.h"

// Screen dimension constants
constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

int
main(int argc, char* args[])
{
    auto mainWindow = MainWindow(SCREEN_WIDTH, SCREEN_HEIGHT);

    mainWindow.runLoop();

    return 0;
}
