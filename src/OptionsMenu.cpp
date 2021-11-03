#include "OptionsMenu.h"

#include "ButtonData.h"

OptionsMenu::OptionsMenu(
    const SDL_Rect& rect,
    SDL_Renderer* renderer,
    TTF_Font* font)
    : rect_(rect)
    , background_(rect, SDL_Color({ 119, 181, 254, 200 }))
    , renderer_(renderer)
{
    // Colors
    // TODO make these a singleton or something
    SDL_Color textColor = { 0, 0, 0, 255 };
    SDL_Color textBackgroundColor = { 255, 255, 255, 255 };
    SDL_Color white = { 255, 255, 255, 255 };

    // Just a rect. Will be overwritten when position sare computed.
    SDL_Rect r = { rect.x, rect.y, 100, 100 };

    // Setup controls to render on top
    controls_.reserve(2);
    controls_.emplace_back(Slider(
        r,
        "Number of Links",
        Slider::Range(1, 4, 1),
        3,
        white,
        renderer,
        font));
    controls_.emplace_back(Slider(
        r,
        "Node Mass (kg)",
        Slider::Range(0.25, 1.5, 0.25),
        0.25,
        white,
        renderer,
        font));

    // Button data
    const std::vector<ButtonData<ButtonId>> buttonDatas{
        { ButtonId::Back, "Back" },
        { ButtonId::Simulate, "Simulate" },
    };

    // Setup each renderable Button
    for (const auto& data : buttonDatas)
    {
        buttons_.emplace_back(
            r,
            static_cast<int>(data.id),
            Text(r, textColor, textBackgroundColor, data.name, renderer, font));
    }

    // Figure out the button positions
    computePositions();
}

void
OptionsMenu::setRect(const SDL_Rect& rect)
{
    rect_ = rect;
    background_.setRect(rect);
    computePositions();
}

void
OptionsMenu::render()
{
    // Render background
    background_.render(renderer_);

    // Render each control
    for (auto& control : controls_)
        control.render();

    // Render each button
    for (auto& button : buttons_)
        button.render();
}

void
OptionsMenu::handleEvent(SDL_Event& e)
{
    for (auto& control : controls_)
    {
        control.handleEvent(e);
        if (control.valueChanged())
        {
            SDL_LogInfo(
                SDL_LOG_CATEGORY_APPLICATION,
                "Control value changed to: %f",
                control.value());
        }
    }

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
                case ButtonId::Back:
                    shouldGoBack_ = true;
                    break;

                case ButtonId::Simulate:
                    shouldSimulate_ = true;
                    break;

                default:
                    break;
            }
        }
    }
}

void
OptionsMenu::clearState()
{
    shouldGoBack_ = false;
    shouldSimulate_ = false;
}

void
OptionsMenu::computePositions()
{
    int optionWidth = 200;
    int optionHeight = 80;

    int controlWidth = 300;
    int controlHeight = 90;

    int margin = 10;

    // Figure out top left x,y for the group of all options
    int totalHeight =
        (optionHeight + margin + margin) * (controls_.size() + buttons_.size());
    int totalControlWidth = controlWidth + margin + margin; // One column
    int totalOptionWidth = optionWidth + margin + margin;   // One column

    int controlX = rect_.x + (0.5 * rect_.w) - (0.5 * totalControlWidth);
    int optionX = rect_.x + (0.5 * rect_.w) - (0.5 * totalOptionWidth);
    int topY = rect_.y + (0.5 * rect_.h) - (0.5 * totalHeight);

    // Track the y value of each control
    // This is updated after each control to know where to place the next one
    int lastTopY = topY;

    // Render controls first
    for (auto& control : controls_)
    {
        int x = controlX + margin;
        int y = lastTopY + margin;

        control.setRect({ x, y, controlWidth, controlHeight });

        lastTopY = y + controlHeight + margin;
    }

    // Then render buttons
    for (auto& button : buttons_)
    {
        int x = optionX + margin;
        int y = lastTopY + margin;

        button.setRect({ x, y, optionWidth, optionHeight });

        lastTopY = y + optionHeight + margin;
    }
}
