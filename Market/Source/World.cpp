/**
* Title: Market
* Author: Tonia Sanzo
* Date: 6/15/21
*
* World is 2D array that represents the game world and the positions of the rugs and npcs
* within that game world.
*/
#include "PCH.h"
#include "World.h"
#include "SDLManager.h"


/**
* Default Constructor, default iniatialize every member variable
*/
World::World()
{
    mWindowWidth  = 0;
    mWindowHeight = 0;
}


/**
* Initialize the World to match the dimensions of the map
*/
bool World::init()
{
    bool success = true;

    mWindowWidth  = SDLManager::mWindowWidth;
    mWindowHeight = SDLManager::mWindowHeight;

    // Confirm the SDLManager window is initialized
    if ((mWindowHeight * mWindowWidth) == 0)
    {
        cout << "Failed to initialize the World!\n";
        success = false;
    }
    else
    {
        // mWorld is a "1D" array that represents a "2D" world
        mWorld.resize((static_cast<size_t>(mWindowWidth) * static_cast<size_t>(mWindowHeight)), nullptr);
        cout << "World size: " << mWorld.size() << "\n";
    }

    return success;
}


/**
* Add an entity to the world at a certain location
*
* @param Entity* - reference to the entity being added to the world
* @param mLocation - location to move the entity to.
*/
void World::addEntity(Entity* aEntity, const Vector3& mLocation)
{
    // the vector index of the given location
    uint32_t locationIndex = (mWindowWidth * static_cast<uint32_t>(mLocation.y)) + static_cast<uint32_t>(mLocation.x);
    if (locationIndex > mWorld.size())
    {
        cout << "locationIndex > mWorld.size()! oh no\n";
    }

    // Determine which partition the entity is being added to,
    // lock that partition and add the element to it.
    if (mLocation.x < mWindowWidth / 3)
    {
        lock_guard<mutex> lk(mLeftMtx);
        Entity* worldEntity = mWorld[locationIndex];
        
        // If the location is empty add the current entity to the location.
        if (worldEntity == nullptr)
        {
            worldEntity = aEntity;
        }

        // Otherwise, grow the linked list of entities at that location, the current
        // entity is the head of the chain.
        else
        {
            aEntity->mNextEntity = worldEntity;
            worldEntity = aEntity;
        }
    }
    else if (mLocation.x > (2 * mWindowWidth) / 3)
    {
        lock_guard<mutex> lk(mCenterMtx);
        Entity* worldEntity = mWorld[locationIndex];

        // If the location is empty add the current entity to the location.
        if (worldEntity == nullptr)
        {
            worldEntity = aEntity;
        }

        // Otherwise, grow the linked list of entities at that location, the current
        // entity is the head of the chain.
        else
        {
            aEntity->mNextEntity = worldEntity;
        }
    }
    else
    {
        lock_guard<mutex> lk(mRightMtx);
        Entity* worldEntity = mWorld[locationIndex];

        // If the location is empty add the current entity to the location.
        if (worldEntity == nullptr)
        {
            worldEntity = aEntity;
        }

        // Otherwise, grow the linked list of entities at that location, the current
        // entity is the head of the chain.
        else
        {
            aEntity->mNextEntity = worldEntity;
        }
    }
}


/**
* Remove an Entity from the world at a certain location
*
* @param Entity* - reference to the Entity being removed from the world
* @param mLocation - location to move the entity to
*/
void World::removeEntity(Entity* aEntity, const Vector3& mLocation)
{
    // the vector index of the given location
    uint32_t locationIndex = (mWindowWidth * static_cast<uint32_t>(mLocation.y)) + static_cast<uint32_t>(mLocation.x);
    if (locationIndex > mWorld.size())
    {
        cout << "locationIndex > mWorld.size()! oh no (removeEntity)\n";
    }

    // Determine which partition the entity is being added to,
    // lock that partition and add the element to it.
    if (mLocation.x < mWindowWidth / 3)
    {
        lock_guard<mutex> lk(mLeftMtx);
        Entity* worldEntity = mWorld[locationIndex];
        
        // If the location is not empty remove the current Entity
        if (worldEntity != nullptr)
        {
            // handle special case when the first element in the linked list is the target Entity
            if (worldEntity == aEntity)
            {
                mWorld[locationIndex] = mWorld[locationIndex]->mNextEntity;
            }
            else
            {
                bool keepSearching = true;
                while(keepSearching)
                {
                    if (worldEntity->mNextEntity == nullptr)
                    {
                        keepSearching = false;
                    }
                    else if (worldEntity->mNextEntity == aEntity)
                    {
                        worldEntity->mNextEntity = worldEntity->mNextEntity->mNextEntity;
                        keepSearching = false;
                    }
                }
            }
        }
    }
    else if (mLocation.x > (2 * mWindowWidth) / 3)
    {
        lock_guard<mutex> lk(mCenterMtx);
        Entity* worldEntity = mWorld[locationIndex];

        // If the location is not empty remove the current Entity
        if (worldEntity != nullptr)
        {
            // handle special case when the first element in the linked list is the target Entity
            if (worldEntity == aEntity)
            {
                mWorld[locationIndex] = mWorld[locationIndex]->mNextEntity;
            }
            else
            {
                bool keepSearching = true;
                while (keepSearching)
                {
                    if (worldEntity->mNextEntity == nullptr)
                    {
                        keepSearching = false;
                    }
                    else if (worldEntity->mNextEntity == aEntity)
                    {
                        worldEntity->mNextEntity = worldEntity->mNextEntity->mNextEntity;
                        keepSearching = false;
                    }
                }
            }
        }
    }
    else
    {
        lock_guard<mutex> lk(mRightMtx);
        Entity* worldEntity = mWorld[locationIndex];

        // If the location is not empty remove the current Entity
        if (worldEntity != nullptr)
        {
            // handle special case when the first element in the linked list is the target Entity
            if (worldEntity == aEntity)
            {
                mWorld[locationIndex] = mWorld[locationIndex]->mNextEntity;
            }
            else
            {
                bool keepSearching = true;
                while (keepSearching)
                {
                    if (worldEntity->mNextEntity == nullptr)
                    {
                        keepSearching = false;
                    }
                    else if (worldEntity->mNextEntity == aEntity)
                    {
                        worldEntity->mNextEntity = worldEntity->mNextEntity->mNextEntity;
                        keepSearching = false;
                    }
                }
            }
        }
    }
}