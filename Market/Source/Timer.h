/**
* Title: Market
* Author: Tonia Sanzo
* Date: 6/19/21
*
* Timer is a class that acts as a timer returing the number of ticks that have
* happened since being started. The timer has the functionality to start, stop,
* and pause
*/
#pragma once
#include "PCH.h"
#include "SDL.h"

class Timer {
public:
    // Initializes variables
    Timer();

    // The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    // Get's the timer's time
    Uint32 getTicks();

    // Checks the status of the timer
    bool isStarted();
    bool isPaused();
private:
    // The clock time when the timer started
    Uint32 mStartTicks;

    // The ticks stored when the timer was paused
    Uint32 mPausedTicks;

    // The timer status
    bool mPaused;
    bool mStarted;
};
