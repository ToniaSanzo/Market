/**
* Title: Market
* Author: Tonia Sanzo
* Date: 6/7/21
*
* pch.h is our precompiled header it contains stable header files that rarely change such as <vector>. It's purpose is
* to increase compilation speed.
*/
#pragma once
using namespace std;    // This set's the namespace for the entire project


// C++ Standard Libraries
#include <string>
#include <iostream>
#include <exception>
#include <cmath>
#include <ctime>
#include <thread>
#include <mutex>
#include <vector>
#include <memory>
#include <random>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <chrono>

// The entities trade states
enum class ETradeState
{
    CHICKEN,
    COW,
    BREAD
};

// Metadata about rug.png spritesheet
#define RUG_FRAMES 3
#define RUG_FRAME_WIDTH 64
#define RUG_FRAME_HEIGHT 64

// Metadata about npc.png spritesheet
#define NPC_FRAMES_COLS 6
#define NPC_FRAMES_ROWS 3
#define NPC_FRAME_WIDTH 96
#define NPC_FRAME_HEIGHT 32
#define NPC_TRADE_FRAMES 2

// Metadata about the beahavior of the NPC
#define MIN_SPEED .3f
#define MAX_SPEED 1.5f
#define MAX_ANIMATION_SPEED .2f
#define NPC_TARGET_LOCATION_RANGE 8

// Absolute value function
#define ABS(N) ((N < 0) ? (-N) : (N))