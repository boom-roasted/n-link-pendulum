#include "Navigator.h"

#include "ButtonData.h"

Navigator::Navigator(const SDL_Rect& rect, SDL_Renderer* renderer)
    : rect_(rect)
    , background_(rect, SDL_Color({ 0, 0, 0, 0 }))
    , renderer_(renderer)
{
    const std::vector<ButtonData<ButtonId>> buttonDatas{
        { ButtonId::ZoomIn, "res/zoomIn.png", SDLK_EQUALS },
        { ButtonId::ZoomOut, "res/zoomOut.png", SDLK_MINUS },
        { ButtonId::ZoomFit, "res/zoomFit.png", SDLK_0 },
    };

    SDL_Color backgroundColor = { 200, 200, 200, 255 };

    // Setup each renderable Button
    buttons_.reserve(buttonDatas.size());
    SDL_Rect r = { rect.x, rect.y, 100, 100 }; // will be overwritten

    for (const auto& data : buttonDatas)
    {
        buttons_.emplace_back(Button(
            r,
            static_cast<int>(data.id),
            Icon(r, backgroundColor, data.name, renderer),
            data.hotkeys));
    }

    // Figure out the button positions
    computePositions();
}

void
Navigator::setRenderer(SDL_Renderer* renderer)
{
    renderer_ = renderer;
    for (auto& button : buttons_)
    {
        button.setRenderer(renderer);
    }
}

void
Navigator::setRect(const SDL_Rect& rect)
{
    rect_ = rect;
    background_.setRect(rect);
    computePositions();
}

void
Navigator::clearState()
{
    shouldZoomIn_ = false;
    shouldZoomOut_ = false;
    shouldZoomFit_ = false;
}

void
Navigator::render()
{
    // Render background
    background_.render(renderer_);

    // Render each option
    for (auto& button : buttons_)
        button.render();
}

void
Navigator::handleEvent(SDL_Event& e)
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
                case ButtonId::ZoomIn:
                    shouldZoomIn_ = true;
                    break;

                case ButtonId::ZoomOut:
                    shouldZoomOut_ = true;
                    break;

                case ButtonId::ZoomFit:
                    shouldZoomFit_ = true;
                    break;

                default:
                    break;
            }
        }
    }
}

void
Navigator::computePositions()
{
    int buttonWidth = 50;
    int buttonHeight = 50;

    int margin = 10;

    int topX = rect_.x + rect_.w - margin - buttonWidth - margin;
    int topY = rect_.y;

    // Track the top left x, y point of each button
    int lastTopX = topX;
    int lastTopY = topY;

    for (auto& button : buttons_)
    {
        int x = lastTopX + margin;
        int y = lastTopY + margin;

        button.setRect({ x, y, buttonWidth, buttonHeight });

        lastTopX = lastTopX; // No update here, just one column
        lastTopY = y + buttonWidth + margin;
    }
}
