/**
* Title: Market
* Author: Tonia Sanzo
* Date: 6/19/21
*
* Timer is a class that acts as a timer returing the number of ticks that have
* happened since being started. The timer has the functionality to start, stop,
* and pause
*/
#include "PCH.h"
#include "Timer.h"

// Init Timer
Timer::Timer() {
    // Initialize the variables
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;
}

// Start the clock
void Timer::start() {
    // Start the timer
    mStarted = true;

    // Unpause the timer
    mPaused = false;

    // Get the current clock time
    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}

// Stop the clock
void Timer::stop() {
    // Stop the timer
    mStarted = false;

    // Unpause the timer
    mPaused = false;

    // Clear tick variables
    mStartTicks = 0;
    mPausedTicks = 0;
}

// Pause the clock
void Timer::pause() {
    // If the timer is running and isn't already paused
    if (mStarted && !mPaused) {
        mPaused = true;

        // Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
        mStartTicks = 0;
    }
}

// Unpause the clock
void Timer::unpause() {
    // aIf the timer is running and paused 
    if (mStarted && mPaused) {
        // Unpause the timer
        mPaused = false;
    }
}

// Get the timer's time
Uint32 Timer::getTicks() {
    // The actual timer time
    Uint32 time = 0;

    // If the timer is running
    if (mStarted) {
        // If the timer is paused
        if (mPaused) {
            time = mPausedTicks;
        }
        else {
            // Returnthe current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return time;
}

// Return if the clock is running or paused
bool Timer::isStarted() {
    // Timer is running and paused or unpaused
    return mStarted;
}

// Return if the clock is paused
bool Timer::isPaused() {
    // Timer is running and paused
    return mPaused && mStarted;
}