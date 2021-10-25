#include "PendulumProvider.h"

#include <filesystem>
#include <limits>

PendulumProvider::PendulumProvider(const SDL_Rect& rect)
    : rect_(rect)
    , pendulumOverTime_(Pendulum::OverTime())
    , pinTexture_(Texture())
    , nodeTexture_(Texture())
    , currentPendulumIndex_(0)
    , lastFrame_(0)
    , scaleFactor_(50)
{
}

void
PendulumProvider::setRect(const SDL_Rect& rect)
{
    rect_ = rect;
    computeScaleFactor();
}

bool
PendulumProvider::loadFromFile(const std::string& p)
{
    bool success = true;

    if (!pendulumOverTime_.empty())
    {
        success = false;
    }
    else if (!std::filesystem::exists(p))
    {
        success = false;
    }
    else
    {
        pendulumOverTime_ = Pendulum::Pendulum::Deserialize(p);
        computeScaleFactor();
    }

    lastFrame_ = pendulumOverTime_.size();

    return success;
}

void
PendulumProvider::loadTextures(SDL_Renderer* renderer)
{
    SDL_assert(pinTexture_.loadFromFile("res/dot.bmp", renderer));
    SDL_assert(nodeTexture_.loadFromFile("res/dot.bmp", renderer));
}

Pendulum::Pendulum
PendulumProvider::currentPendulum()
{
    return pendulumOverTime_[currentPendulumIndex_];
}

void
PendulumProvider::restart()
{
    currentPendulumIndex_ = 0;
}

void
PendulumProvider::incrementFrame(std::size_t by)
{
    if (currentPendulumIndex_ + by < lastFrame_)
        currentPendulumIndex_ += by;
    else
        restart();
}

void
PendulumProvider::decrementFrame(std::size_t by)
{
    if (static_cast<int>(currentPendulumIndex_ - by) > 0)
        currentPendulumIndex_ -= by;
    else
        restart();
}

void
PendulumProvider::incrementTime(double by)
{
    const auto targetTime = currentPendulum().time() + by;

    for (std::size_t i = currentPendulumIndex_; i < lastFrame_; i++)
    {
        if (pendulumOverTime_[i].time() > targetTime)
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
PendulumProvider::zoomFit()
{
    computeScaleFactor();
}

void
PendulumProvider::zoom(double factor)
{
    scaleFactor_ *= factor;
}

void
PendulumProvider::render(SDL_Renderer* renderer)
{
    if (pendulumOverTime_.empty())
        return;

    // The offsets determine the pin position, which is the
    // origin of the pendulum coordinate system.
    const double offsetX = xOrigin();
    const double offsetY = yOrigin();

    // The scale factor will stretch the nodes apart. We want
    // the scale factor to be sized such that the bottom node
    // almost touches the bottom of the screen
    const double sf = scaleFactor_;

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

int
PendulumProvider::xOrigin()
{
    return 0.5 * rect_.w; // Center
}

int
PendulumProvider::yOrigin()
{
    return 0.15 * rect_.h; // Near the top
}

void
PendulumProvider::computeScaleFactor()
{
    if (pendulumOverTime_.empty())
        return;

    double xMin = 0, xMax = 0;
    double yMin = 0, yMax = 0;

    for (const auto& p : pendulumOverTime_)
    {
        for (const auto& n : p.nodes())
        {
            if (n.state.y > yMax)
                yMax = n.state.y;

            if (n.state.y < yMin)
                yMin = n.state.y;

            if (n.state.x > xMax)
                xMax = n.state.x;

            if (n.state.x < xMin)
                xMin = n.state.x;
        }
    }

    int padding = 20;
    int x0 = xOrigin();
    int y0 = yOrigin();

    const double doubleMaxLimit = std::numeric_limits<double>::max();

    // clang-format off
    double sfLeft = xMin < 0 ? (x0 - rect_.x - padding) / -xMin : doubleMaxLimit;
    double sfTop  = yMin < 0 ? (y0 - rect_.y + padding) / -yMin : doubleMaxLimit;

    double sfRight  = xMax > 0 ? (rect_.x + rect_.w - x0 + padding) / xMax : doubleMaxLimit;
    double sfBottom = yMax > 0 ? (rect_.y + rect_.h - y0 - padding) / yMax : doubleMaxLimit;
    // clang-format on

    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "Computed Ideal Scale Factors {%f, %f, %f, %f}",
        sfLeft,
        sfTop,
        sfRight,
        sfBottom);

    double sfMin = std::numeric_limits<double>::max();
    for (const auto& sf : { sfTop, sfBottom, sfLeft, sfRight })
        if (std::abs(sf) < sfMin)
            sfMin = sf;

    scaleFactor_ = sfMin;
}