#include "PendulumProvider.h"

#include <filesystem>

PendulumProvider::PendulumProvider(const SDL_Rect& rect)
    : rect_(rect)
    , pendulumOverTime(Pendulum::OverTime())
    , pinTexture_(Texture())
    , nodeTexture_(Texture())
    , currentPendulumIndex_(0)
    , lastFrame_(0)
{
}

void
PendulumProvider::setRect(const SDL_Rect& rect)
{
    rect_ = rect;
}

bool
PendulumProvider::loadFromFile(const std::string& p)
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

    lastFrame_ = pendulumOverTime.size();

    return success;
}

void
PendulumProvider::loadTextures(SDL_Renderer* renderer)
{
    pinTexture_.loadFromFile("res/dot.bmp", renderer);
    nodeTexture_.loadFromFile("res/dot.bmp", renderer);
    // TODO add asserts or something
}

Pendulum::Pendulum
PendulumProvider::currentPendulum()
{
    return pendulumOverTime[currentPendulumIndex_];
}

void
PendulumProvider::incrementFrame(std::size_t by)
{
    if (currentPendulumIndex_ + by < lastFrame_)
        currentPendulumIndex_ += by;
    else
        currentPendulumIndex_ = 0; // Reset
}

void
PendulumProvider::incrementTime(double by)
{
    const auto targetTime = currentPendulum().time() + by;

    for (std::size_t i = currentPendulumIndex_; i < lastFrame_; i++)
    {
        if (pendulumOverTime[i].time() > targetTime)
        {
            // We found it
            currentPendulumIndex_ = i;
            return;
        }
    }

    // If we got here, it means the target time is out of bounds of the
    // simulation. Reset.
    currentPendulumIndex_ = 0;
}

void
PendulumProvider::render(SDL_Renderer* renderer, double sf)
{
    if (pendulumOverTime.empty())
        return;

    // The offsets determine the pin position, which is the
    // origin of the pendulum coordinate system.
    const double offsetX = 0.5 * rect_.w;  // Center
    const double offsetY = 0.15 * rect_.h; // Near the top

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