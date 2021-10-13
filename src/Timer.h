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

    // Gets the timer's time
    Uint32 getTicks();

    // Checks timer status
    bool isStarted();
    bool isPaused();

private:
    // The clock time when the timer started
    Uint32 startTicks_;

    // The ticks stored when the timer was paused
    Uint32 pausedTicks_;

    // The timer status
    bool isPaused_;
    bool isStarted_;
};
