#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <SDL2/SDL.h>

#include "Menu.h"
#include "PendulumProvider.h"
#include "Renderer.h"
#include "Texture.h"
#include "Window.h"

class MainWindow
{
public:
    MainWindow(int w, int h);

    // Deallocates memory
    // Frees media and shuts down SDL
    ~MainWindow();

    // Starts up SDL and creates window
    bool init();

    // Loads media
    bool loadMedia();

    // Main event loop
    void runLoop();

private:
    // The window we'll be rendering to
    Window window_;

    // The window renderer
    Renderer renderer_;

    // Scene textures
    Texture fpsTextTexture_;

    // Fonts
    TTF_Font* mainFont_;

    // The pendulum
    PendulumProvider pendulumProvider_;

    // Menu overlays
    // TODO maybe this should just be an optional?
    // Or even just a normal struct, but add a `bool MenuVisible` member?
    std::vector<MainMenu> menus_;

    // Screen dimensions
    int w_;
    int h_;
};

#endif // MAINWINDOW_H