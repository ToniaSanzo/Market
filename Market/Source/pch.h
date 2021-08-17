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
    * Determine if two points are in range of one another using the distance squared formula.
    * The distance squared formula is the Pythagorean Theorem but skips the square root step for optimization
    * taking the square root for performance.
    * @param aPointA     - First target location.
    * @param aPointB     - Second target location.
    * @param aRange      - The distance for an overlap to occur.
    * @return bool True if the locations are within the range of one another, otherwise false.
    */
    inline bool pointsInRange(const Vector& aPointA, const Vector& aPointB, const float& aRange)
    {
        if ((pow(aPointA.x - aPointB.x, 2.f) + pow(aPointA.y - aPointB.y, 2.f)) <= pow(aRange, 2.f))
        {
            return true;
        }
        else
        {
            return false;
        }
    }


    /**
    * Takes three vectors, two being the start and end of the line segment and the other being the intersection
    * point. Returns true if the intersection is between the line segment points, otherwise returns false.
    * @param aLineSegmentPointA - Endpoint of the aLineSegmentPoint1.
    * @param aLineSegmentPointB - Endpoint of the aLineSegmentPoint2.
    * @return bool True if the intersection point is between the line segment points, otherwise false.
    */
    inline bool intersectionBetweenPoints(const Vector& aLineSegmentPointA, const Vector& aLineSegmentPointB, const Vector& aIntersectionPoint)
    {
        Vector xValuesMinMax = aLineSegmentPointA.x < aLineSegmentPointB.x ? (Vector{ aLineSegmentPointA.x, aLineSegmentPointB.x, 0 }) : (Vector{ aLineSegmentPointB.x, aLineSegmentPointA.x, 0 });
        Vector yValuesMinMax = aLineSegmentPointA.y < aLineSegmentPointB.y ? (Vector{ aLineSegmentPointA.y, aLineSegmentPointB.y, 0 }) : (Vector{ aLineSegmentPointB.y, aLineSegmentPointA.y, 0 });

        if ((aIntersectionPoint.x >= xValuesMinMax.x && aIntersectionPoint.x <= xValuesMinMax.y)
            && (aIntersectionPoint.y >= yValuesMinMax.x && aIntersectionPoint.y <= yValuesMinMax.y))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    /**
    * Determine if a line segment overlaps a circle.
    * @param aLineSegmentPointA - The first point of the line segment.
    * @param aLineSegmentPointB - The second point of the line segment.
    * @param aCircleCenterPoint - The circles center point.
    * @param aCircleRadius      - The circle's radius.
    * @return bool True if the circle overlaps the line segment, otherwise false.
    */
    inline bool doesLineSegmentOverlapCircle(Vector aLineSegmentPointA, Vector aLineSegmentPointB, Vector aCircleCenterPoint, float aCircleRadius)
    {
        if (pointsInRange(aLineSegmentPointA, aCircleCenterPoint, aCircleRadius) || pointsInRange(aLineSegmentPointB, aCircleCenterPoint, aCircleRadius))
        {
            return true;
        }

        float lineSegmentSlope = (aLineSegmentPointB.y - aLineSegmentPointA.y) / (aLineSegmentPointB.x - aLineSegmentPointA.x);
        float perpendicularLineSegmentSlope = -(((aLineSegmentPointB.x - aLineSegmentPointA.x)) / (aLineSegmentPointB.y - aLineSegmentPointA.y));
        float lineSegmentYIntercept = aLineSegmentPointA.y - (lineSegmentSlope * aLineSegmentPointA.x);
        float perpendicularLineSegmentYIntercept = aCircleCenterPoint.y - (perpendicularLineSegmentSlope * aCircleCenterPoint.x);

        Vector intersectionPoint;
        intersectionPoint.x = (perpendicularLineSegmentYIntercept - lineSegmentYIntercept) / (lineSegmentSlope - perpendicularLineSegmentSlope);
        intersectionPoint.y = (lineSegmentSlope * intersectionPoint.x) + lineSegmentYIntercept;


        if (pointsInRange(aCircleCenterPoint, intersectionPoint, aCircleRadius))
        {
            return intersectionBetweenPoints(aLineSegmentPointA, aLineSegmentPointB, intersectionPoint);
        }
        // The intersection point is out of range of the circle's overlap, therefore return false 
        else
        {
            return false;
        }
    }
}

// Number of each entity types in game world (rugs, npcs)
constexpr uint16_t ENTITY_COUNT              = 40;

// Metadata about rug.png spritesheet
constexpr uint16_t RUG_FRAME_COLS            = 3;
constexpr uint16_t RUG_FRAME_ROWS            = 2;
constexpr uint16_t RUG_FRAME_WIDTH           = 49;
constexpr uint16_t RUG_FRAME_HEIGHT          = 49;
constexpr uint16_t RUG_SCALE                 = 3;

// Metadata about npc.png spritesheet
constexpr uint16_t NPC_FRAME_COLS            = 6;
constexpr uint16_t NPC_FRAME_ROWS            = 3;
constexpr uint16_t NPC_FRAME_WIDTH           = 18;
constexpr uint16_t NPC_FRAME_HEIGHT          = 28;
constexpr uint16_t NPC_TRADE_FRAMES          = 2;
constexpr uint16_t NPC_SCALE                 = 3;
constexpr float NPC_RESET_DIRECTION_TIME     = 1.32f;

// Metadata about the behavior of the NPC
constexpr float MIN_SPEED                    = 125.f;
constexpr float MAX_SPEED                    = 300.f;
constexpr float MAX_ANIMATION_SPEED          = .15f;
constexpr float MIN_ANIMATION_SPEED          = .55f;
constexpr uint16_t NPC_TARGET_LOCATION_RANGE = 18;

// Metadata about the dimensions of the background
constexpr uint16_t BACKGROUND_WIDTH          = 640;
constexpr uint16_t BACKGROUND_HEIGHT         = 380;

// Metadata about the loading screen
constexpr uint8_t LOAD_BLOOD_FRAME_COLS      = 3;
constexpr uint8_t LOAD_BLOOD_FRAME_ROWS      = 9;
constexpr uint8_t TOTAL_LOAD_BLOOD_FRAMES    = (LOAD_BLOOD_FRAME_COLS * LOAD_BLOOD_FRAME_ROWS) - 2;
constexpr float LOAD_ANIMATION_SPEED         = .08;

// Number of NPCs and rugs to generate
constexpr uint16_t NPC_COUNT                 = 5;
constexpr uint16_t RUG_COUNT                 = 3;

// Max amount of in game time that can pass for a single game loop
constexpr float MAX_FRAME_TIME               = .5f;

// Time needed to pass before a rug can trade with a NPC
constexpr float RUG_TRADE_TIME               = .5f;
// Radius the Entity's have to be within one another to trade
constexpr float TRADE_RADIUS                 = 50.f;

// Number of partitions that make up the world
constexpr float PARTITION_COUNT              = 3.f;

// Absolute value function
#define ABS(N) ((N < 0) ? (-N) : (N))