#include "RenderableChain.h"

#include <filesystem>

RenderableChain::RenderableChain()
{
    pendulumOverTime = Pendulum::OverTime();
    pinTexture_ = Texture();
    nodeTexture_ = Texture();
    currentPendulumIndex_ = 0;
}

RenderableChain::~RenderableChain() {}

bool
RenderableChain::loadFromFile(const std::string& p)
{
    bool success = true;

    if (!pendulumOverTime.empty())
    {
        success = false;
    }
    else if (!std::filesystem::exists(p))
    {
        success = false;
    }
    else
    {
        pendulumOverTime = Pendulum::Pendulum::Deserialize(p);
    }

    return success;
}

void
RenderableChain::loadTextures(SDL_Renderer* renderer)
{
    pinTexture_.loadFromFile("res/dot.bmp", renderer);
    nodeTexture_.loadFromFile("res/dot.bmp", renderer);
    // TODO add asserts or something
}

Pendulum::Pendulum
RenderableChain::currentPendulum()
{
    return pendulumOverTime[currentPendulumIndex_];
}

void
RenderableChain::increment(int by)
{
    const auto maxIndex = static_cast<int>(pendulumOverTime.size());
    if (currentPendulumIndex_ + by < maxIndex)
        currentPendulumIndex_ += by;
    else
        currentPendulumIndex_ = 0; // Reset
}

void
RenderableChain::render(
    SDL_Renderer* renderer,
    double offsetX,
    double offsetY,
    double sf)
{
    if (pendulumOverTime.empty())
        return;

    const auto& chain = currentPendulum();

    const auto applyScalingX = [offsetX, sf](double x)
    { return x * sf + offsetX; };
    const auto applyScalingY = [offsetY, sf](double y)
    { return y * sf + offsetY; };

    // Show the pin
    const auto pinX = applyScalingX(chain.pin().x);
    const auto pinY = applyScalingY(chain.pin().y);
    pinTexture_.render(pinX, pinY, renderer, true);

    // Show the nodes.
    // Need to track position of previous point to draw connecting lines
    const auto nodes = chain.nodes();
    auto lastPosition = Point(pinX, pinY);

    for (std::size_t i = 0; i < nodes.size(); i++)
    {
        const auto& node = nodes[i];

        const auto thisX = applyScalingX(node.state.x);
        const auto thisY = applyScalingY(node.state.y);

        // Draw the node right here
        nodeTexture_.render(thisX, thisY, renderer, true);

        // Draw connecting line
        // Line width isn't controllable without sdl2_gfx
        // TODO Instead can make a rectangle
        SDL_SetRenderDrawColor(renderer, 0x00, 100, 149, 237);
        SDL_RenderDrawLine(
            renderer, lastPosition.x, lastPosition.y, thisX, thisY);

        // Update the previous point
        lastPosition.x = thisX;
        lastPosition.y = thisY;
    }
}