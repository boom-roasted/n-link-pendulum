#include "MainWindow.h"

#include "Dot.h"
#include "FpsCounter.h"
#include "Timer.h"

MainWindow::MainWindow(int w, int h)
    : window_(NULL)
    , renderer_(NULL)
    , dotTexture_(Texture())
    , mainFont_(NULL)
    , pendulumProvider_(PendulumProvider({ 0, 0, w, h }))
    , menus_(std::vector<MainMenu>())
    , w_(w)
    , h_(h)
{
}

MainWindow::~MainWindow()
{
    // Free loaded images
    dotTexture_.free();

    // Free fonts
    TTF_CloseFont(mainFont_);
    mainFont_ = NULL;

    // Menus should destroy themselves

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
    mainFont_ = TTF_OpenFont("res/DejaVuSerif.ttf", 16);
    if (mainFont_ == NULL)
    {
        printf("Failed to load ttf font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }

    // Load chain
    if (!pendulumProvider_.loadFromFile("data.bin"))
    {
        printf("Failed to load chain data!\n");
        success = false;
    }

    pendulumProvider_.loadTextures(renderer_);

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
            Timer timer;
            timer.start();

            // Fps counter
            FpsCounter fpsCounter({ 0, 0, w_, h_ });
            fpsCounter.start();

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

                    // User requests main menu
                    if (e.type == SDL_KEYUP && e.key.repeat == 0 &&
                        e.key.keysym.sym == SDLK_ESCAPE)
                    {
                        if (menus_.empty())
                        {
                            // Could use null for automatic whole screen
                            SDL_Rect screenRect = { 0, 0, w_, h_ };

                            // Constructs menu in place with forwarded params
                            menus_.emplace_back(screenRect);
                        }
                        else
                        {
                            menus_.pop_back();
                        }
                    }

                    if (e.type == SDL_WINDOWEVENT)
                    {
                        switch (e.window.event)
                        {
                            // TODO consider not rendering as much when window
                            // is minimized or something
                            case SDL_WINDOWEVENT_RESIZED:
                            case SDL_WINDOWEVENT_SIZE_CHANGED:
                                SDL_LogInfo(
                                    SDL_LOG_CATEGORY_APPLICATION,
                                    "Window %d resized to %dx%d\n",
                                    e.window.windowID,
                                    e.window.data1,
                                    e.window.data2);
                                w_ = e.window.data1;
                                h_ = e.window.data2;
                                dot.setBounds(w_, h_);
                                pendulumProvider_.setRect({ 0, 0, w_, h_ });
                                fpsCounter.setRect({ 0, 0, w_, h_ });
                                for (auto& menu : menus_)
                                    menu.setRect({ 0, 0, w_, h_ });
                                break;
                            default:
                                break;
                        }
                    }

                    // Only handle scene events when no menu is up
                    if (menus_.empty())
                    {
                        // Dot event
                        dot.handleEvent(e);
                    }
                    else
                    {
                        // Menu event
                        bool shouldResume;
                        menus_.back().handleEvent(e, shouldResume, quit);
                        if (shouldResume)
                            menus_.pop_back();
                    }
                }

                // Time since last render frame (seconds)
                float deltaT = timer.lap() / 1000.f;

                // Freeze scene interaction when menus are presented
                if (menus_.empty())
                {
                    // Move the dot
                    dot.move();

                    // Increment the chain position
                    pendulumProvider_.incrementTime(deltaT);
                }

                // Clear screen
                SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(renderer_);

                // Render dot
                dot.render(dotTexture_, renderer_);

                // Render pendulum
                pendulumProvider_.render(renderer_);

                // Render fps counter
                fpsCounter.render(renderer_, mainFont_);

                // Render any menus
                if (!menus_.empty())
                {
                    (menus_.back()).render(renderer_, mainFont_);
                }

                // Update screen
                SDL_RenderPresent(renderer_);
            }
        }
    }

    // Resources are freeded and SDL is closed on destruction
}