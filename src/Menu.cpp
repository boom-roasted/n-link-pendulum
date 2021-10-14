#include "Menu.h"

MainMenu::MainMenu(SDL_Rect rect)
    : background_(rect, SDL_Color({ 119, 181, 254, 200 }))
{
    const auto textNames =
        std::vector<std::string>{ "Resume", "Options", "Exit" };

    SDL_Color textColor = { 0, 0, 0, 255 };
    SDL_Color textBackgroundColor = { 255, 255, 255, 255 };

    int optionWidth = 200;
    int optionHeight = 80;

    int margin = 20;

    // Figure out top left x,y for the group of all options
    int totalHeight = (optionHeight + margin + margin) * textNames.size();
    int totalWidth = optionWidth + margin + margin; // One column

    int topX = rect.x + (0.5 * rect.w) - (0.5 * totalWidth);
    int topY = rect.y + (0.5 * rect.h) - (0.5 * totalHeight);

    // Setup each renderable Text
    texts_ = std::vector<Text>();
    texts_.reserve(3);

    int lastTopX = topX;
    int lastTopY = topY;

    for (const auto& name : textNames)
    {
        int x = lastTopX + margin;
        int y = lastTopY + margin;

        texts_.push_back(Text(
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
    for (auto& text : texts_)
        text.render(renderer, font);
}
