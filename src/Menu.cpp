#include "Menu.h"

MainMenu::MainMenu(SDL_Rect rect)
    : background_(rect, SDL_Color({ 119, 181, 254, 200 }))
{
    const auto buttonNames =
        std::vector<std::string>{ "Resume", "Options", "Exit" };

    SDL_Color textColor = { 0, 0, 0, 255 };
    SDL_Color textBackgroundColor = { 255, 255, 255, 255 };

    int optionWidth = 200;
    int optionHeight = 80;

    int margin = 20;

    // Figure out top left x,y for the group of all options
    int totalHeight = (optionHeight + margin + margin) * buttonNames.size();
    int totalWidth = optionWidth + margin + margin; // One column

    int topX = rect.x + (0.5 * rect.w) - (0.5 * totalWidth);
    int topY = rect.y + (0.5 * rect.h) - (0.5 * totalHeight);

    // Setup each renderable Text
    buttons_ = std::vector<Button>();
    buttons_.reserve(3);

    int lastTopX = topX;
    int lastTopY = topY;

    for (const auto& name : buttonNames)
    {
        int x = lastTopX + margin;
        int y = lastTopY + margin;

        buttons_.push_back(Button(
            { x, y, optionWidth, optionHeight },
            textColor,
            textBackgroundColor,
            name));

        lastTopX = lastTopX; // No update here, just one column
        lastTopY = y + optionHeight + margin;
    }
}

void
MainMenu::render(SDL_Renderer* renderer, TTF_Font* font)
{
    // Render background
    background_.render(renderer);

    // Render each option
    for (auto& button : buttons_)
        button.render(renderer, font);
}

void
MainMenu::handleEvent(SDL_Event& e)
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
            // TODO attach actions to different buttons
            // button.action();
        }
    }
}
