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

        //Starts up SDL and creates window
        bool init();

        //Loads media
        bool loadMedia();

        //Frees media and shuts down SDL
		void Close();

        // Main event loop
        int MainLoop();

	private:
        //The window we'll be rendering to
        SDL_Window* gWindow;

        //The window renderer
        SDL_Renderer* gRenderer;

        //Scene textures
        LTexture gDotTexture;

		// Screen dimensions
		int screenWidth;
		int screenHeight;
};

#endif // MAINWINDOW_H