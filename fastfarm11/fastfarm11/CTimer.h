#ifndef _TIMER_H_
#define _TIMER_H_

#include <SDL.h>
/**
*  What we think our timer should look like
*/
class CTimer {
public:
    CTimer();
    ///Start the timer
    void Start();
    ///Stop the timer
    void Stop();
    ///Pause the timer
    void Pause();
    ///Unpause the timer
    void Unpause();
    /**
    *  Restart the timer and return the elapsed ticks
    *  @return The elapsed ticks
    */
    int Restart();
    /**
    *  Get the elapsed ticks
    *  @return The elapsed ticks
    */
    int Ticks() const;
    ///Check if Timer is started
    bool Started() const;
    ///Check if Timer is paused
    bool Paused() const;

private:
    int mStartTicks, mPausedTicks;
    bool mStarted, mPaused;
};

#endif