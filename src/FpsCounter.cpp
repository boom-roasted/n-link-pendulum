#include "FpsCounter.h"

FpsCounter::FpsCounter(SDL_Rect rect)
    : rect_(rect)
    , timer_(Timer())
    , texture_(Texture())
    , color_({ 0, 0, 0, 0 })
    , text_("")
    , countedFrames_(0)
{
}

void
FpsCounter::start()
{
    timer_.start();
}

void
FpsCounter::render(SDL_Renderer* renderer, TTF_Font* font)
{
    // Calculate fps. If there was a very small time delta, fps
    // might be too high.
    float avgFPS = countedFrames_ / (timer_.getTicks() / 1000.f);
    if (avgFPS > 2000000)
        avgFPS = 0;

    // Render fps text
    // Set text to be rendered
    text_.str("");
    text_ << "Avg FPS " << avgFPS;

    // Render text
    if (!texture_.loadFromRenderedText(
            renderer, text_.str().c_str(), font, color_))
        printf("Unable to render FPS texture!\n");

    constexpr int padding = 20;

    // Render fps text, aligned to the bottom left
    texture_.render(
        rect_.x + padding,
        rect_.y - padding + (rect_.h - texture_.getHeight()),
        renderer);

    // Count this frame. Could reset every once and a while to prevent
    // overflow, but doesn't really matter. It'll mess up the average
    // but okay. The value will wrap at around 2^64-1.
    // But compiler dependent: defined in <climits
    // Note that n % m == n & (m - 1) where m is a power of 2
    ++countedFrames_;
}