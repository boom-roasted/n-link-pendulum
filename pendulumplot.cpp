#include <fstream>
#include <stdio.h>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "chain.h"
#include "plot/MainWindow.h"

// Screen dimension constants
constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;

int
main(int argc, char* args[])
{
    auto mainWindow = MainWindow(SCREEN_WIDTH, SCREEN_HEIGHT);

    mainWindow.MainLoop();

    return 0;
}
