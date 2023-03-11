#include "MainWindow.h"

#include <stdexcept>

#include "String.h"

MainWindow::MainWindow(int w, int h)
    : sdlLib_(SdlLib())
    , sdlImg_(SdlImg(IMG_INIT_PNG))
    , sdlTtf_(SdlTtf())
    , window_(NULL)
    , renderer_(NULL)
    , mainFont_(Font("res/DejaVuSerif.ttf", 16))
    , pendulumProvider_(PendulumProvider({ 0, 0, w, h }))
    , menus_(std::vector<MainMenu>())
    , timer_(Timer())
    , fpsCounter_(FpsCounter({ 0, 0, w_, h_ }))
    , playback_(Playback({ 0, 0, w_, h_ }, renderer_))
    , navigator_(Navigator({ 0, 0, w_, h_ }, renderer_))
    , w_(w)
    , h_(h)
{
    // The actual window.
    window_ = Window(
        "Pendulum",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        w_,
        h_,
        SDL_WINDOW_SHOWN);

    // A vsynced renderer for window.
    renderer_ = Renderer(
        window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Initialize renderer color
    SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
}

void
MainWindow::loadMedia()
{
    // Load existing simulation or run new one
    pendulumProvider_.loadOrCreate("data.bin");

    // Load pendulum textures
    pendulumProvider_.loadTextures(renderer_);
}

void
MainWindow::runLoop()
{
    // Load media
    loadMedia();

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    // Timers
    timer_.start();
    fpsCounter_.start();

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
                        fpsCounter_.setRect(r);
                        playback_.setRect(r);
                        navigator_.setRect(r);
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
                playback_.handleEvent(e);

                // Navigator event
                navigator_.handleEvent(e);
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
                    timer_.restart();
                }

                // Remove menu or at least clear the event state
                if (menus_.back().shouldResume())
                    menus_.pop_back();
                else
                    menus_.back().clearState();
            }
        }

        // Time since last render frame (seconds)
        float deltaT = timer_.lap() / 1000.f;

        // Only allow scene interaction when menus are hidden
        if (menus_.empty())
        {
            // Handle zooming and other viewport navigation
            if (navigator_.shouldZoomFit())
                pendulumProvider_.zoomFit();
            else if (navigator_.shouldZoomIn())
                pendulumProvider_.zoom(1.1);
            else if (navigator_.shouldZoomOut())
                pendulumProvider_.zoom(0.9);
            navigator_.clearState();

            // Chain movement is controlled by the playback controller
            if (playback_.shouldRestart())
                pendulumProvider_.restart();

            if (!playback_.isPaused())
            {
                // Increment the chain position
                pendulumProvider_.incrementTime(deltaT);
            }
            else
            {
                // Playback is paused, but frame by frame is allowed
                if (playback_.shouldFrameBack())
                    pendulumProvider_.decrementFrame(10);
                if (playback_.shouldFrameForward())
                    pendulumProvider_.incrementFrame(10);
            }
        }

        // Playback state has been processed
        playback_.clearState();

        // Clear screen
        SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer_);

        // Render pendulum
        pendulumProvider_.render(renderer_);

        // Render fps counter
        fpsCounter_.render(renderer_, mainFont_);

        // Render playback controls
        playback_.render();

        // Render navigation controls
        navigator_.render();

        // Render any menus
        if (!menus_.empty())
        {
            (menus_.back()).render();
        }

        // Update screen
        SDL_RenderPresent(renderer_);
    }

    // Resources are freeded and SDL is closed on destruction
}