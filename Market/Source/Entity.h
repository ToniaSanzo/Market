/**
* Title: Market
* Author: Tonia Sanzo
* Date: 6/15/21
*
* Entity is a wrapper class for game entities, that guarentee we will be able to derive the
* subclass of the object
*/
#pragma once
#include "PCH.h"


// The different subclasses that inherit from Entity
enum class EEntityType
{
    NPC,
    RUG
};


// Abstract base class that represents different game entities, this class guarentees we will be able to 
// safely cast to the derived base class given an Entity object.
class Entity
{
public:
    // Returns the entity type of the derived class
    virtual EEntityType getType() = 0;

    // Render the entity to the screen
    virtual void render() = 0;
};