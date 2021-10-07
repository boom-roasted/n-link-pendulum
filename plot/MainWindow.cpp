#include "MainWindow.h"

#include "Dot.h"

MainWindow::MainWindow(int w, int h)
{
    gWindow = NULL;
    gRenderer = NULL;
    gDotTexture = Texture();
    screenWidth = w;
    screenHeight = h;
}

MainWindow::~MainWindow()
{
    Close();
}

bool
MainWindow::init()
{
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    } else {
        // Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf("Warning: Linear texture filtering not enabled!");
        }

        // Create window
        gWindow = SDL_CreateWindow("SDL Tutorial",
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   screenWidth,
                                   screenHeight,
                                   SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL Error: %s\n",
                   SDL_GetError());
            success = false;
        } else {
            // Create vsynced renderer for window
            gRenderer = SDL_CreateRenderer(gWindow,
                                           -1,
                                           SDL_RENDERER_ACCELERATED |
                                             SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n",
                       SDL_GetError());
                success = false;
            } else {
                // Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                // Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf(
                      "SDL_image could not initialize! SDL_image Error: %s\n",
                      IMG_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

bool
MainWindow::loadMedia()
{
    // Loading success flag
    bool success = true;

    // Load dot texture
    if (!gDotTexture.loadFromFile("dot.bmp", gRenderer)) {
        printf("Failed to load dot texture!\n");
        success = false;
    }

    return success;
}

void
MainWindow::Close()
{
    // Free loaded images
    gDotTexture.free();

    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

int
MainWindow::MainLoop()
{
    // Start up SDL and create window
    if (!init()) {
        printf("Failed to initialize!\n");
    } else {
        // Load media
        if (!loadMedia()) {
            printf("Failed to load media!\n");
        } else {
            // Main loop flag
            bool quit = false;

            // Event handler
            SDL_Event e;

            // The dot that will be moving around on the screen
            Dot dot(screenWidth, screenHeight);

            // While application is running
            while (!quit) {
                // Handle events on queue
                while (SDL_PollEvent(&e) != 0) {
                    // User requests quit
                    if (e.type == SDL_QUIT ||
                        (e.type == SDL_KEYUP && e.key.repeat == 0 &&
                         e.key.keysym.sym == SDLK_q)) {
                        quit = true;
                    }

                    // Handle input for the dot
                    dot.handleEvent(e);
                }

                // Move the dot
                dot.move();

                // Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                // Render objects
                dot.render(gDotTexture, gRenderer);

                // Update screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }

    // Free resources and close SDL
    // close(); called on destruction

    return 0;
}