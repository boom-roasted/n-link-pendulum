#include "MainWindow.h"

#include "Dot.h"

MainWindow::MainWindow(int w, int h)
{
    window_ = NULL;
    renderer_ = NULL;
    dotTexture_ = Texture();
    screenWidth_ = w;
    screenHeight_ = h;
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
        window_ = SDL_CreateWindow(
            "SDL Tutorial",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            screenWidth_,
            screenHeight_,
            SDL_WINDOW_SHOWN);
        if (window_ == NULL) {
            printf(
                "Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            // Create vsynced renderer for window
            renderer_ = SDL_CreateRenderer(
                window_,
                -1,
                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (renderer_ == NULL) {
                printf(
                    "Renderer could not be created! SDL Error: %s\n",
                    SDL_GetError());
                success = false;
            } else {
                // Initialize renderer color
                SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);

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
    if (!dotTexture_.loadFromFile("dot.bmp", renderer_)) {
        printf("Failed to load dot texture!\n");
        success = false;
    }

    return success;
}

void
MainWindow::Close()
{
    // Free loaded images
    dotTexture_.free();

    // Destroy window
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    window_ = NULL;
    renderer_ = NULL;

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
            Dot dot(screenWidth_, screenHeight_);

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
                SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(renderer_);

                // Render objects
                dot.render(dotTexture_, renderer_);

                // Update screen
                SDL_RenderPresent(renderer_);
            }
        }
    }

    // Free resources and close SDL
    // close(); called on destruction

    return 0;
}