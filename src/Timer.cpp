#include "Timer.h"

Timer::Timer()
{
    // Initially timer clock has no time,
    // the timer is not paused, and the
    // timer is not started.
    startTicks_ = 0;
    pausedTicks_ = 0;
    lapTicks_ = 0;

    isPaused_ = false;
    isStarted_ = false;
}

void
Timer::start()
{
    // Start the timer
    isStarted_ = true;

    // Unpause the timer
    isPaused_ = false;

    // Get the current clock time
    startTicks_ = SDL_GetTicks();
    pausedTicks_ = 0;
}

void
Timer::stop()
{
    // Stop the timer
    isStarted_ = false;

    // Unpause the timer
    isPaused_ = false;

    // Clear tick variables
    startTicks_ = 0;
    pausedTicks_ = 0;
}

void
Timer::pause()
{
    // If the timer is running and isn't already paused
    if (isStarted_ && !isPaused_)
    {
        // Pause the timer
        isPaused_ = true;

        // Calculate the paused ticks
        pausedTicks_ = SDL_GetTicks() - startTicks_;
        startTicks_ = 0;
    }
}

void
Timer::unpause()
{
    // If the timer is running and paused
    if (isStarted_ && isPaused_)
    {
        // Unpause the timer
        isPaused_ = false;

        // Reset the starting ticks
        startTicks_ = SDL_GetTicks() - pausedTicks_;

        // Reset the paused ticks
        pausedTicks_ = 0;
    }
}

void
Timer::restart()
{
    start();
}

Uint32
Timer::getTicks()
{
    // The actual timer time
    Uint32 time = 0;

    // If the timer is running
    if (isStarted_)
    {
        // If the timer is paused
        if (isPaused_)
        {
            // Return the number of ticks when the timer was paused
            time = pausedTicks_;
        }
        else
        {
            // Return the current time minus the start time
            time = SDL_GetTicks() - startTicks_;
        }
    }

    return time;
}

Uint32
Timer::lap()
{
    Uint32 lapTime = 0;
    if (isStarted_)
    {
        if (isPaused_)
        {
            // Time between the last lap and when we paused
            lapTime = pausedTicks_ - lapTicks_;
            lapTicks_ = pausedTicks_;
        }
        else
        {
            auto currentTicks = SDL_GetTicks();
            lapTime = currentTicks - lapTicks_;
            lapTicks_ = currentTicks;
        }
    }
    return lapTime;
}

bool
Timer::isStarted()
{
    // Timer is running and paused or unpaused
    return isStarted_;
}

bool
Timer::isPaused()
{
    // Timer is running and paused
    return isPaused_ && isStarted_;
}
