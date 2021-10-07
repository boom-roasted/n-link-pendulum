#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <SDL2/SDL.h>

#include "Texture.h"

class MainWindow
{
public:
    MainWindow(int w, int h);

    // Deallocates memory
    ~MainWindow();

    // Starts up SDL and creates window
    bool init();

    // Loads media
    bool loadMedia();

    // Frees media and shuts down SDL
    void Close();

    // Main event loop
    int MainLoop();

private:
    // The window we'll be rendering to
    SDL_Window* window_;

    // The window renderer
    SDL_Renderer* renderer_;

    // Scene textures
    Texture dotTexture_;

    // Screen dimensions
    int screenWidth_;
    int screenHeight_;
};

#endif // MAINWINDOW_H