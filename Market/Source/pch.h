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
#include <atomic>

// The entities trade states
enum class ETradeState
{
    CHICKEN,
    LAMB,
    BREAD_WINE
};

// 3D vector
struct Vector
{
    float x = 0;
    float y = 0;
    float z = 0;

    Vector operator-(const Vector& aVector)
    {
        Vector returnValue;
        returnValue.x = this->x - aVector.x;
        returnValue.y = this->y - aVector.y;
        returnValue.z = this->z - aVector.z;
        return returnValue;
    }

    Vector operator+(const Vector& aVector)
    {
        Vector returnValue;
        returnValue.x = this->x + aVector.x;
        returnValue.y = this->y + aVector.y;
        returnValue.z = this->z + aVector.z;
        return returnValue;
    }
};

// Math function
namespace MATH
{
    /**
    * Keeps a Vector value clamped in the range specified by the argument. The aValue range is updated by
    reference.
    *
    * @param aValue - The given value will be kept between 0, and the corresponding {x,y,z} range specified by
    *                 aRange
    * @param aRange - The max values for eacch {x,y,z} value of aValue
    * @return aValue clamped between 0 and aRange
    */
    inline void clamp(Vector& aValue, const Vector& aRange)
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


    /**
    * Keeps a float value clamped in the range specified by the argument. The aValue range is updated by
    * reference.
    *
    * @param aValue - The given value will be kept between 0, and the corresponding range specified by
    *                 aRange
    * @param aRange - The max values for each {x,y,z} value of aValue
    * @return float of aValue clamped between 0 and aRange
    */
    inline float clamp(float& aValue, const float& aRange)
    {
        if (aValue < 0)
        {
            aValue = 0;
        }
        else if (aValue >= aRange)
        {
            aValue = aRange;
        }
        else
        {
            aValue = aValue;
        }

        return aValue;
    }


    /**
    * Determine if two locations are in range of one another using the distance squared formula.
    * The distance squared formula is the pythagrom theorem withough taking the square root for
    * performance.
    *
    * @param aLocationA     - First target location.
    * @param aLocationB     - Second target location.
    * @param aRadiusSquared - The squared radius for an overlap to occur squared.
    * @return bool true if the locations are within the nonsquared radius of one another.
    */
    inline bool distanceSquared(const Vector& aLocationA, const Vector& aLocationB, const float& aRadiusSquared)
    {
        // If the locations are within the radius of one another return true
        if ((pow(aLocationA.x - aLocationB.x, 2.f) + pow(aLocationA.y - aLocationB.y, 2.f)) <= aRadiusSquared)
        {
            return true;
        }
        // Otherwise return false
        else
        {
            return false;
        }
    }
}

// Number of each entity types in game world (rugs, npcs)
constexpr uint16_t ENTITY_COUNT = 90;

// Metadata about rug.png spritesheet
constexpr uint16_t RUG_FRAME_COLS   = 3;
constexpr uint16_t RUG_FRAME_ROWS   = 2;
constexpr uint16_t RUG_FRAME_WIDTH  = 49;
constexpr uint16_t RUG_FRAME_HEIGHT = 49;
constexpr uint16_t RUG_SCALE        = 3;

// Metadata about npc.png spritesheet
constexpr uint16_t NPC_FRAME_COLS        = 6;
constexpr uint16_t NPC_FRAME_ROWS        = 3;
constexpr uint16_t NPC_FRAME_WIDTH       = 18;
constexpr uint16_t NPC_FRAME_HEIGHT      = 28;
constexpr uint16_t NPC_TRADE_FRAMES      = 2;
constexpr uint16_t NPC_SCALE             = 3;
constexpr float NPC_RESET_DIRECTION_TIME = 1.32f;

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

// Max amount of in game time that can pass for a single game loop
constexpr float MAX_FRAME_TIME = .5f;

// Time needed to pass before a rug can trade with a NPC
constexpr float RUG_TRADE_TIME = .2f;
// Radius the Entity's have to be within one another to trade
constexpr float TRADE_RADIUS_SQUARED = 650.f;
constexpr Vector RUG_OFFSET = Vector{ 3, 14, 0 };

// Number of partitions that make up the world
constexpr float PARTITION_COUNT = 3.f;

// Absolute value function
#define ABS(N) ((N < 0) ? (-N) : (N))