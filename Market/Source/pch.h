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
constexpr uint16_t RUG_FRAMES       = 3;
constexpr uint16_t RUG_FRAME_WIDTH  = 64;
constexpr uint16_t RUG_FRAME_HEIGHT = 64;

// Metadata about npc.png spritesheet
constexpr uint16_t NPC_FRAMES_COLS  = 6;
constexpr uint16_t NPC_FRAMES_ROWS  = 3;
constexpr uint16_t NPC_FRAME_WIDTH  = 96;
constexpr uint16_t NPC_FRAME_HEIGHT = 32;
constexpr uint16_t NPC_TRADE_FRAMES = 2;

// Metadata about the beahavior of the NPC
constexpr float MIN_SPEED               = 125.f;
constexpr float MAX_SPEED               = 330.f;
constexpr float MAX_ANIMATION_SPEED     = .15f;
constexpr float MIN_ANIMATION_SPEED     = .55f;
constexpr uint16_t NPC_TARGET_LOCATION_RANGE = 8;

// Number of NPCs and rugs to generate
constexpr uint16_t NPC_COUNT = 5;
constexpr uint16_t RUG_COUNT = 3;

// Absolute value function
#define ABS(N) ((N < 0) ? (-N) : (N))


// 3D vector
struct Vector3
{
    float x = 0;
    float y = 0;
    float z = 0;
};