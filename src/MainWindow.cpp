#include "MainWindow.h"

#include "FpsCounter.h"
#include "Navigator.h"
#include "Playback.h"
#include "Timer.h"

MainWindow::MainWindow(int w, int h)
    : window_(NULL)
    , renderer_(NULL)
    , mainFont_(NULL)
    , pendulumProvider_(PendulumProvider({ 0, 0, w, h }))
    , menus_(std::vector<MainMenu>())
    , w_(w)
    , h_(h)
{
}

MainWindow::~MainWindow()
{
    // Free fonts
    TTF_CloseFont(mainFont_);
    mainFont_ = NULL;

    // Menus should destroy themselves

    // Destroy renderer. Could let destructor handle this.
    renderer_.dispose();

    // Destroy window. Could let destructor handle this.
    window_.dispose();

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
            SDL_LogWarn(
                SDL_LOG_CATEGORY_VIDEO,
                "Warning: Linear texture filtering not enabled!");
        }

        window_ = Window(
            "SDL Tutorial",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            w_,
            h_,
            SDL_WINDOW_SHOWN);

        // Create vsynced renderer for window
        renderer_ = Renderer(
            window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

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

    return success;
}

bool
MainWindow::loadMedia()
{
    // Loading success flag
    bool success = true;

    // Load font
    // Open the font
    mainFont_ = TTF_OpenFont("res/DejaVuSerif.ttf", 16);
    if (mainFont_ == NULL)
    {
        printf("Failed to load ttf font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }

    // Load existing simulation or run new one
    pendulumProvider_.loadOrCreate("data.bin");

    // Load pendulum textures
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

            // Playback control
            Playback playback({ 0, 0, w_, h_ }, renderer_);

            // Navigation control
            Navigator navigator({ 0, 0, w_, h_ }, renderer_);

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
                            menus_.emplace_back(
                                screenRect,
                                pendulumProvider_.options(),
                                renderer_,
                                mainFont_);
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
                            {
                                SDL_LogInfo(
                                    SDL_LOG_CATEGORY_APPLICATION,
                                    "Window %d resized to %dx%d\n",
                                    e.window.windowID,
                                    e.window.data1,
                                    e.window.data2);
                                w_ = e.window.data1;
                                h_ = e.window.data2;
                                SDL_Rect r = { 0, 0, w_, h_ };
                                pendulumProvider_.setRect(r);
                                fpsCounter.setRect(r);
                                playback.setRect(r);
                                navigator.setRect(r);
                                for (auto& menu : menus_)
                                    menu.setRect(r);
                                break;
                            }
                            default:
                                break;
                        }
                    }

                    // Only handle scene events when no menu is up
                    if (menus_.empty())
                    {
                        // Playback event
                        playback.handleEvent(e);

                        // Navigator event
                        navigator.handleEvent(e);
                    }
                    else
                    {
                        // Menu event
                        menus_.back().handleEvent(e);

                        if (menus_.back().shouldQuit())
                            quit = true;

                        if (menus_.back().shouldSimulate())
                        {
                            pendulumProvider_.runSimulation(
                                menus_.back().pendulumOptions());

                            // The simulate call blocks.
                            // TODO remove timer restart after adding async
                            timer.restart();
                        }

                        // Remove menu or at least clear the event state
                        if (menus_.back().shouldResume())
                            menus_.pop_back();
                        else
                            menus_.back().clearState();
                    }
                }

                // Time since last render frame (seconds)
                float deltaT = timer.lap() / 1000.f;

                // Only allow scene interaction when menus are hidden
                if (menus_.empty())
                {
                    // Handle zooming and other viewport navigation
                    if (navigator.shouldZoomFit())
                        pendulumProvider_.zoomFit();
                    else if (navigator.shouldZoomIn())
                        pendulumProvider_.zoom(1.1);
                    else if (navigator.shouldZoomOut())
                        pendulumProvider_.zoom(0.9);
                    navigator.clearState();

                    // Chain movement is controlled by the playback controller
                    if (playback.shouldRestart())
                        pendulumProvider_.restart();

                    if (!playback.isPaused())
                    {
                        // Increment the chain position
                        pendulumProvider_.incrementTime(deltaT);
                    }
                    else
                    {
                        // Playback is paused, but frame by frame is allowed
                        if (playback.shouldFrameBack())
                            pendulumProvider_.decrementFrame(1);
                        if (playback.shouldFrameForward())
                            pendulumProvider_.incrementFrame(1);
                    }
                }

                // Playback state has been processed
                playback.clearState();

                // Clear screen
                SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(renderer_);

                // Render pendulum
                pendulumProvider_.render(renderer_);

                // Render fps counter
                fpsCounter.render(renderer_, mainFont_);

                // Render playback controls
                playback.render();

                // Render navigation controls
                navigator.render();

                // Render any menus
                if (!menus_.empty())
                {
                    (menus_.back()).render();
                }

                // Update screen
                SDL_RenderPresent(renderer_);
            }
        }
    }

    // Resources are freeded and SDL is closed on destruction
}