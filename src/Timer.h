#ifndef PENDULUM_TIMER_H
#define PENDULUM_TIMER_H

#include <SDL2/SDL.h>

class Timer
{
public:
    Timer();

    // Clock actions
    void start();
    void stop();
    void pause();
    void unpause();
    void restart();

    // Gets the timer's time
    Uint32 getTicks();

    // Gets the time since the last time lap was checked
    Uint32 lap();

    // Checks timer status
    bool isStarted();
    bool isPaused();

private:
    // The clock time when the timer started
    Uint32 startTicks_;

    // The ticks stored when the timer was paused
    Uint32 pausedTicks_;

    // The ticks for whenever a lap is recorded
    Uint32 lapTicks_;

    // The timer status
    bool isPaused_;
    bool isStarted_;
};

#endif // PENDULUM_TIMER_H
