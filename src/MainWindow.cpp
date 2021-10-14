#include "MainWindow.h"

#include <sstream>

#include "Dot.h"
#include "Timer.h"

MainWindow::MainWindow(int w, int h)
{
    window_ = NULL;
    renderer_ = NULL;
    dotTexture_ = Texture();
    fpsTextTexture_ = Texture();
    fpsFont_ = NULL;
    renderableChain_ = RenderableChain();
    w_ = w;
    h_ = h;
}

MainWindow::~MainWindow()
{
    // Free loaded images
    dotTexture_.free();
    fpsTextTexture_.free();

    // Free fonts
    TTF_CloseFont(fpsFont_);
    fpsFont_ = NULL;

    // Destroy window
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    window_ = NULL;
    renderer_ = NULL;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

bool
MainWindow::init()
{
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        // Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }

        // Create window
        window_ = SDL_CreateWindow(
            "SDL Tutorial",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            w_,
            h_,
            SDL_WINDOW_SHOWN);
        if (window_ == NULL)
        {
            printf(
                "Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            // Create vsynced renderer for window
            renderer_ = SDL_CreateRenderer(
                window_,
                -1,
                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (renderer_ == NULL)
            {
                printf(
                    "Renderer could not be created! SDL Error: %s\n",
                    SDL_GetError());
                success = false;
            }
            else
            {
                // Initialize renderer color
                SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);

                // Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf(
                        "SDL_image could not initialize! SDL_image Error: %s\n",
                        IMG_GetError());
                    success = false;
                }

                // Initialize ttf library
                if (TTF_Init() == -1)
                {
                    printf(
                        "SDL_ttf could not initialize! SDL_ttf Error: %s\n",
                        TTF_GetError());
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
    if (!dotTexture_.loadFromFile("res/dot.bmp", renderer_))
    {
        printf("Failed to load dot texture!\n");
        success = false;
    }

    // Load font
    // Open the font
    fpsFont_ = TTF_OpenFont("res/DejaVuSerif.ttf", 16);
    if (fpsFont_ == NULL)
    {
        printf("Failed to load ttf font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }

    // Load chain
    if (!renderableChain_.loadFromFile("data.bin"))
    {
        printf("Failed to load chain data!\n");
        success = false;
    }

    renderableChain_.loadTextures(renderer_);

    return success;
}

void
MainWindow::runLoop()
{
    // Start up SDL and create window
    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        // Load media
        if (!loadMedia())
        {
            printf("Failed to load media!\n");
        }
        else
        {
            // Main loop flag
            bool quit = false;

            // Event handler
            SDL_Event e;

            // Timer
            int countedFrames = 0;
            Timer fpsTimer;
            fpsTimer.start();
            std::stringstream fpsText;

            // Text color
            SDL_Color textColor = { 0, 0, 0, 255 };

            // The dot that will be moving around on the screen
            Dot dot(w_, h_);

            // While application is running
            while (!quit)
            {
                // Handle events on queue
                while (SDL_PollEvent(&e) != 0)
                {
                    // User requests quit
                    if (e.type == SDL_QUIT ||
                        (e.type == SDL_KEYUP && e.key.repeat == 0 &&
                         e.key.keysym.sym == SDLK_q))
                    {
                        quit = true;
                    }

                    // Handle input for the dot
                    dot.handleEvent(e);
                }

                // Calculate fps. If there was a very small time delta, fps
                // might be too high.
                float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
                if (avgFPS > 2000000)
                {
                    avgFPS = 0;
                }

                // Move the dot
                dot.move();

                // Increment the chain position
                renderableChain_.increment(500);

                // Clear screen
                SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(renderer_);

                // Render dot
                dot.render(dotTexture_, renderer_);

                // Render pendulum
                renderableChain_.render(renderer_, 0.5 * w_, 0.15 * h_, 50);

                // Render fps text
                // Set text to be rendered
                fpsText.str("");
                fpsText << "Average Frames Per Second " << avgFPS;

                // Render text
                if (!fpsTextTexture_.loadFromRenderedText(
                        renderer_, fpsText.str().c_str(), fpsFont_, textColor))
                {
                    printf("Unable to render FPS texture!\n");
                }

                // Render fps text, aligned to the bottom left
                fpsTextTexture_.render(
                    20, -20 + (h_ - fpsTextTexture_.getHeight()), renderer_);

                // Update screen
                SDL_RenderPresent(renderer_);

                // Count this frame. TODO what if this overflows the int?
                ++countedFrames;
            }
        }
    }

    // Resources are freeded and SDL is closed on destruction
}