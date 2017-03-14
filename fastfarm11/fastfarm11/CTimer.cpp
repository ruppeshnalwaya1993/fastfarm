#include "CTimer.h"

CTimer::CTimer()
    : mStartTicks(0), mPausedTicks(0), mStarted(false), mPaused(false)
{
}

void CTimer::Start() {
    mStarted = true;
    mPaused  = false;
    mStartTicks = SDL_GetTicks();
}
void CTimer::Stop() {
    mStarted = false;
    mPaused  = false;
}

void CTimer::Pause() {
    if (mStarted && !mPaused) {
        mPaused = true;
        mPausedTicks = SDL_GetTicks() - mStartTicks;
    }
}

void CTimer::Unpause() {
    if (mPaused) {
        mPaused = false;
        mStartTicks = SDL_GetTicks() - mPausedTicks;
        mPausedTicks = 0;
    }
}

int CTimer::Restart() {
    int elapsedTicks = Ticks();
    Start();
    return elapsedTicks;
}

int CTimer::Ticks() const {
    if (mStarted) {
        if (mPaused)
            return mPausedTicks;
        else
            return SDL_GetTicks() - mStartTicks;
    }
    return 0;
}

bool CTimer::Started() const {
    return mStarted;
}

bool CTimer::Paused() const {
    return mPaused;
}