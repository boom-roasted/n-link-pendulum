#include "Playback.h"

Playback::Playback(const SDL_Rect& rect)
    : rect_(rect)
    , background_(rect, SDL_Color({ 0, 0, 0, 0 })) // Transparent
{
    const std::vector<ButtonData> buttonDatas{
        { ButtonId::PlayPause, "Play/Pause" },
        { ButtonId::Restart, "Restart" },
        { ButtonId::FrameBack, "<<" },
        { ButtonId::FrameForward, ">>" },
    };

    SDL_Color textColor = { 0, 0, 0, 255 };
    SDL_Color textBackgroundColor = { 200, 200, 200, 255 };

    // Setup each renderable Button
    buttons_.reserve(buttonDatas.size());
    SDL_Rect r = { rect.x, rect.y, 100, 100 }; // will be overwritten

    for (const auto& data : buttonDatas)
    {
        buttons_.emplace_back(
            r,
            static_cast<int>(data.id),
            data.name,
            textColor,
            textBackgroundColor);
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

void
Playback::render(SDL_Renderer* renderer, TTF_Font* font)
{
    // Render background
    background_.render(renderer);

    // Render each option
    for (auto& button : buttons_)
        button.render(renderer, font);
}

std::optional<Playback::ButtonId>
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

            return static_cast<ButtonId>(button.id());
        }
    }

    return std::nullopt;
}

void
Playback::computePositions()
{
    int buttonWidth = 100;
    int buttonHeight = 50;

    int margin = 20;

    // Figure out top left x,y for the group of all options
    // int totalHeight = buttonHeight + margin + margin;
    // int totalWidth = (buttonWidth + margin + margin) * buttons_.size();

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
