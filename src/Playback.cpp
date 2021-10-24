#include "Playback.h"

Playback::Playback(const SDL_Rect& rect, SDL_Renderer* renderer)
    : rect_(rect)
    , background_(rect, SDL_Color({ 0, 0, 0, 0 }))
    , renderer_(renderer)
{
    const std::vector<ButtonData> buttonDatas{
        { ButtonId::PlayPause, "res/play.png" },
        { ButtonId::Restart, "res/restart.png" },
        { ButtonId::FrameBack, "res/framebackward.png" },
        { ButtonId::FrameForward, "res/frameforward.png" },
    };

    SDL_Color backgroundColor = { 200, 200, 200, 255 };

    // Setup each renderable Button
    buttons_.reserve(buttonDatas.size());
    SDL_Rect r = { rect.x, rect.y, 100, 100 }; // will be overwritten

    for (const auto& data : buttonDatas)
    {
        buttons_.emplace_back(
            r,
            static_cast<int>(data.id),
            Icon(r, backgroundColor, data.name, renderer));
    }

    // Figure out the button positions
    computePositions();
}

void
Playback::setRect(const SDL_Rect& rect)
{
    rect_ = rect;
    background_.setRect(rect);
    computePositions();
}

bool
Playback::isPaused()
{
    return isPaused_;
}

bool
Playback::shouldRestart()
{
    return shouldRestart_;
}

bool
Playback::shouldFrameBack()
{
    return shouldFrameBack_;
}

bool
Playback::shouldFrameForward()
{
    return shouldFrameForward_;
}

void
Playback::clearState()
{
    shouldRestart_ = false;
    shouldFrameBack_ = false;
    shouldFrameForward_ = false;
}

void
Playback::render()
{
    // Render background
    background_.render(renderer_);

    // Render each option
    for (auto& button : buttons_)
        button.render();
}

void
Playback::handleEvent(SDL_Event& e)
{
    for (auto& button : buttons_)
    {
        button.handleEvent(e);
        if (button.wasClicked())
        {

            SDL_LogInfo(
                SDL_LOG_CATEGORY_APPLICATION,
                "Button '%s' was clicked",
                button.text().c_str());

            switch (static_cast<ButtonId>(button.id()))
            {
                case ButtonId::PlayPause:
                    isPaused_ = !isPaused_;
                    break;

                case ButtonId::Restart:
                    shouldRestart_ = true;
                    break;

                case ButtonId::FrameBack:
                    shouldFrameBack_ = true;
                    break;

                case ButtonId::FrameForward:
                    shouldFrameForward_ = true;
                    break;

                default:
                    break;
            }
        }
    }
}

void
Playback::computePositions()
{
    int buttonWidth = 50;
    int buttonHeight = 50;

    int margin = 10;

    int topX = rect_.x; // + margin;
    int topY = rect_.y; // + margin;

    // Track the top left x, y point of each button
    int lastTopX = topX;
    int lastTopY = topY;

    for (auto& button : buttons_)
    {
        int x = lastTopX + margin;
        int y = lastTopY + margin;

        button.setRect({ x, y, buttonWidth, buttonHeight });

        lastTopX = x + buttonWidth + margin;
        lastTopY = lastTopY; // No update here ,just one row
    }
}
