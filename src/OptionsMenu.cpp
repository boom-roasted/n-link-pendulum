#include "OptionsMenu.h"

#include "ButtonData.h"

OptionsMenu::OptionsMenu(
    const SDL_Rect& rect,
    const PendulumOptions& pendulumOptions,
    SDL_Renderer* renderer,
    TTF_Font* font)
    : rect_(rect)
    , background_(rect, SDL_Color({ 119, 181, 254, 200 }))
    , renderer_(renderer)
    , pendulumOptions_(pendulumOptions)
{
    // Colors
    // TODO make these a singleton or something
    SDL_Color textColor = { 0, 0, 0, 255 };
    SDL_Color textBackgroundColor = { 255, 255, 255, 255 };
    SDL_Color white = { 255, 255, 255, 255 };

    // Just a rect. Will be overwritten when position sare computed.
    SDL_Rect r = { rect.x, rect.y, 100, 100 };

    // Setup controls to render on top
    const std::vector<SliderData<ControlId>> sliderDatas{
        {
            ControlId::NumLinks,
            "Number of Links",
            Slider::Range(1, 10, 1),
            pendulumOptions.numLinks,
        },
        {
            ControlId::SimTime,
            "Simulation Length (seconds)",
            Slider::Range(5, 60, 5),
            pendulumOptions.simTime,
        },
        {
            ControlId::m,
            "Node Mass (kg)",
            Slider::Range(0.1, 1, 0.1),
            pendulumOptions.m,
        },
        {
            ControlId::l,
            "Link Length (m)",
            Slider::Range(1, 10, 1),
            pendulumOptions.l,
        },
        {
            ControlId::k,
            "Spring Constant (kN/m)",
            Slider::Range(60, 160, 20),
            pendulumOptions.k * 1e-3, // Convert N/m to kN/m}
        },
        {
            ControlId::c,
            "Dampening (micro N/m/m)",
            Slider::Range(50, 200, 50),
            pendulumOptions.c * 1e6, // Convert N/m/m to micro N/m/m
        },
    };

    for (const auto& data : sliderDatas)
    {
        controls_.emplace_back(
            r,
            static_cast<int>(data.id),
            data.label,
            data.range,
            data.initialValue,
            white,
            renderer,
            font);
    }
    controls_.reserve(sliderDatas.size());

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

            switch (static_cast<ControlId>(control.id()))
            {
                case ControlId::NumLinks:
                    pendulumOptions_.numLinks = control.value();
                    break;

                case ControlId::SimTime:
                    pendulumOptions_.simTime = control.value();
                    break;

                case ControlId::m:
                    pendulumOptions_.m = control.value();
                    break;

                case ControlId::l:
                    pendulumOptions_.l = control.value();
                    break;

                case ControlId::k:
                    // Convert kN/m to N/m
                    pendulumOptions_.k = control.value() * 1e3;
                    break;

                case ControlId::c:
                    // Convert micro N/m/m to N/m/m
                    pendulumOptions_.c = control.value() * 1e-6;
                    break;

                default:
                    break;
            }
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
    int buttonWidth = 100;
    int buttonHeight = 40;

    int controlWidth = 300;
    int controlHeight = 65;

    int margin = 10;

    // Figure out top left x,y for the group of all options
    // One button row, multiple control rows.
    int totalHeight = (controlHeight + margin + margin) * controls_.size() +
                      (buttonHeight + margin + margin) * 1;
    int totalControlWidth = controlWidth + margin + margin; // One column

    int controlX = rect_.x + (0.5 * rect_.w) - (0.5 * totalControlWidth);
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

    // Then render buttons in a horizontal stack
    const int totalButtonWidth =
        (buttonWidth + margin + margin) * buttons_.size(); // One row
    int buttonX = rect_.x + (0.5 * rect_.w) - (0.5 * totalButtonWidth);
    const auto buttonY = lastTopY;

    for (auto& button : buttons_)
    {
        int x = buttonX + margin;
        int y = buttonY + margin;

        button.setRect({ x, y, buttonWidth, buttonHeight });

        buttonX = x + buttonWidth + margin;
    }
}
