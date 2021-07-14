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

// Number of each entity types in game world (rugs, npcs)
constexpr uint16_t ENTITY_COUNT = 81;

// Metadata about rug.png spritesheet
constexpr uint16_t RUG_FRAME_COLS   = 3;
constexpr uint16_t RUG_FRAME_ROWS   = 2;
constexpr uint16_t RUG_FRAME_WIDTH  = 64;
constexpr uint16_t RUG_FRAME_HEIGHT = 64;
constexpr uint16_t RUG_SCALE        = 3;

// Metadata about npc.png spritesheet
constexpr uint16_t NPC_FRAME_COLS       = 6;
constexpr uint16_t NPC_FRAME_ROWS       = 3;
constexpr uint16_t NPC_FRAME_WIDTH       = 96;
constexpr uint16_t NPC_FRAME_HEIGHT      = 32;
constexpr uint16_t NPC_TRADE_FRAMES      = 2;
constexpr uint16_t NPC_SCALE             = 3;
constexpr float NPC_RESET_DIRECTION_TIME = 1.3f;

// Metadata about the behavior of the NPC
constexpr float MIN_SPEED                    = 125.f;
constexpr float MAX_SPEED                    = 330.f;
constexpr float MAX_ANIMATION_SPEED          = .15f;
constexpr float MIN_ANIMATION_SPEED          = .55f;
constexpr uint16_t NPC_TARGET_LOCATION_RANGE = 18;

// Metadata about the dimensions of the background
constexpr uint16_t BACKGROUND_WIDTH  = 640;
constexpr uint16_t BACKGROUND_HEIGHT = 380;

// Number of NPCs and rugs to generate
constexpr uint16_t NPC_COUNT = 5;
constexpr uint16_t RUG_COUNT = 3;

// Number of partitions that make up the world
constexpr float PARTITION_COUNT = 3.f;

// Absolute value function
#define ABS(N) ((N < 0) ? (-N) : (N))

// 3D vector
struct Vector3
{
    float x = 0;
    float y = 0;
    float z = 0;
};


// Math function
namespace MATH
{
    //// given to numbers determine the GCD of them using 
    //// the Euclidean algorithm
    //int16_t getGCD(int16_t a, int16_t b)
    //{
    //    int r = a % b;
    //    if (!r)
    //    {
    //        return b;
    //    }
    //    getGCD(b, r);
    //}

    //// Reduce a fraction to it's least common denominator
    //Vector3 reduceFraction(int16_t aNumerator, int16_t aDenominator)
    //{
    //    int gcd = getGCD(aNumerator, aDenominator);
    //    return Vector3{ static_cast<float>(aNumerator / gcd), static_cast<float>(aDenominator / gcd), 0 };
    //}

    /**
    * Returns a Vector3 clamped in the range specified by the argument
    * 
    * @param aValue - The given value will be kept between 0, and the corresponding {x,y,z} range specified by aRange
    * @param aRange - The max values for eacch {x,y,z} value of aValue
    * @return aValue clamped between 0 and aRange
    */
    inline void clamp(Vector3& aValue, const Vector3& aRange)
    {
        if (aValue.x < 0)
        {
            aValue.x = 0;
        }
        else if (aValue.x >= aRange.x)
        {
            aValue.x = aRange.x;
        }
        else
        {
            aValue.x = aValue.x;
        }

        if (aValue.y < 0)
        {
            aValue.y = 0;
        }
        else if (aValue.y >= aRange.y)
        {
            aValue.y = aRange.y;
        }
        else
        {
            aValue.y = aValue.y;
        }
    }
}