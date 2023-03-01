#ifndef PENDULUM_MAINWINDOW_H
#define PENDULUM_MAINWINDOW_H

#include <SDL2/SDL.h>

#include "Menu.h"
#include "PendulumProvider.h"
#include "Renderer.h"
#include "Texture.h"
#include "Window.h"
#include "SdlLib.h"
#include "SdlImg.h"
#include "SdlTtf.h"
#include "Font.h"

class MainWindow
{
public:
    // Sets up SDL.
    // C library calls (that require init/quit) are managed by
    // C++ wrappers which automatically free the memory on destruction.
    MainWindow(int w, int h);

    // Main event loop
    void runLoop();

private:
    // Loads media
    void loadMedia();

    // The sdl library (handles spinning up/down)
    SdlLib sdlLib_;
    SdlImg sdlImg_;
    SdlTtf sdlTtf_;

    // The window we'll be rendering to
    Window window_;

    // The window renderer
    Renderer renderer_;

    // Scene textures
    Texture fpsTextTexture_;

    // Fonts
    Font mainFont_;

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

#endif // PENDULUM_MAINWINDOW_H
