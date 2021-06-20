/**
* Title: Market
* Author: Tonia Sanzo
* Date: 6/7/21
*
* pch.h is our precompiled header it contains stable header files that rarely change such as <vector>. It's purpose is
* to increase compilation speed.
*/
#pragma once
using namespace std;


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


// The current state of a rug or a npc
enum class ETradeState
{
    CHICKEN,
    COW,
    BREAD
};


// Preprocessor macros
#define RUG_FRAMES 3
#define RUG_FRAME_WIDTH 64
#define RUG_FRAME_HEIGHT 64
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 760
#define MIN_SPEED 10.f
#define MAX_SPEED 30.f
#define MAX_ANIMATION_SPEED .2f
#define NPC_FRAMES_COLS 6
#define NPC_FRAMES_ROWS 3
#define NPC_FRAME_WIDTH 96
#define NPC_FRAME_HEIGHT 32
#define NPC_TRADE_FRAMES 2