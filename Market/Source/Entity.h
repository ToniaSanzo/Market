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
    // Linked list of entities, with the same pixel coordinate
    Entity* mNextEntity = nullptr;

    // Only need to set mNextEntity to nullptr since it's deallocated elsewhere in the
    // program
    ~Entity()
    {
        if (mNextEntity)
        {
            mNextEntity = nullptr;
        }
    }

    // Returns the entity type of the derived class
    virtual EEntityType getType() = 0;

    // Return an Entity pointer, to the object the function was called on
    virtual Entity* getEntity() = 0;

    // Render the entity to the screen
    virtual void render() = 0;

};