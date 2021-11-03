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
    , lastUsedOptions_(PendulumOptions())
{
}

void
PendulumProvider::setRect(const SDL_Rect& rect)
{
    rect_ = rect;
    computeScaleFactor();
}

void
PendulumProvider::loadFromFile(const std::string& p)
{
    // Remove existing data
    if (!pendulumOverTime_.empty())
        pendulumOverTime_.clear();

    // Make sure file exists
    SDL_assert(std::filesystem::exists(p));

    pendulumOverTime_ = Pendulum::Pendulum::Deserialize(p);
    computeScaleFactor();

    // Reset index bounds
    currentPendulumIndex_ = 0;
    lastFrame_ = pendulumOverTime_.size();
}

void
PendulumProvider::loadTextures(SDL_Renderer* renderer)
{
    SDL_assert(pinTexture_.loadFromFile("res/pin.png", renderer));
    SDL_assert(nodeTexture_.loadFromFile("res/dot.bmp", renderer));
}

void
PendulumProvider::loadOrCreate(const std::string& p)
{
    // File does exist, so load it
    if (std::filesystem::exists(p))
    {
        loadFromFile(p);
    }

    // File does not exist, so create it
    else
    {
        runSimulation(lastUsedOptions_);
    }
}

void
PendulumProvider::runSimulation(const PendulumOptions& options)
{
    lastUsedOptions_ = options; // Record options

    const std::string fp = "data.bin"; // Output data file

    const double deltaT =
        1.0 / 200.0 * 1.0 /
        std::sqrt(options.k / options.m); // Time step increment
    const int iterations = std::lround(options.simTime / deltaT);

    auto chain = Pendulum::Pendulum::Create(
        options.numLinks,
        options.m,
        options.l,
        options.k,
        options.c,
        Pendulum::Pendulum::Layout::Line);

    // Data storage
    auto fout = std::ofstream(fp, std::ios::out | std::ios::binary);
    if (!fout)
    {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM,
            "Cannot open file for writing! Path: %s",
            fp.c_str());
        SDL_assert(false); // Data storage file could not be opened
    }

    // Write initial state to file
    chain.Serialize(fout);

    // Main simulation loop
    for (int i = 0; i < iterations; i++)
    {
        // Increment time
        chain.RungeKuttaSecondOrder(deltaT);

        // Write state to file. Not every frame is written, because
        // that is too much data.
        if (i % options.saveFrameStep == 0)
        {
            chain.Serialize(fout);
        }

        // Display progress
        if (i % (iterations / 20) == 0)
        {
            double progress = i / static_cast<double>(iterations);
            const int barWidth = 70;
            std::cout << "[";
            int barPosition = barWidth * progress;
            for (int i = 0; i < barWidth; ++i)
            {
                if (i <= barPosition)
                    std::cout << "#";
                else
                    std::cout << " ";
            }
            std::cout << "] " << int(std::lround(progress * 100.0)) << " %\r";
            std::cout.flush();
        }
    }

    // Finish progress bar
    std::cout << std::endl;

    // Close data file
    fout.close();

    // Load the data into the interface
    loadFromFile(fp);
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