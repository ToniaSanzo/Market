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
{}

// Initialize the mWorld vector
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
void World::addEntity(Entity* mEntity, const Vector3& mLocation)
{
    EWorldPartition partition;

    // Determine which partition the entity is being added to,
    // lock that partition and add the element to it.
    if (mLocation.x < mWindowWidth / 3)
    {
        lock_guard<mutex> lk(mLeftMtx);
        
        // If the location is empty add the current entity to the location.
        if ((mWorld[(mWindowWidth * mLocation.y) + mLocation.x]) == nullptr)
        {
            mWorld[(mWindowWidth * mLocation.y) + mLocation.x] = mEntity;
        }

        // Otherwise, grow the linked list of entities at that location, the current
        // entity is the head of the chain.
        else
        {
            mEntity->mNextEntity = mWorld[(mWindowWidth * mLocation.y) + mLocation.x];
            mWorld[(mWindowWidth * mLocation.y) + mLocation.x] = mEntity;
        }
    }
    else if (mLocation.x > (2 * mWindowWidth) / 3)
    {
        lock_guard<mutex> lk(mCenterMtx);

        // If the location is empty add the current entity to the location.
        if ((mWorld[(mWindowWidth * mLocation.y) + mLocation.x]) == nullptr)
        {
            mWorld[(mWindowWidth * mLocation.y) + mLocation.x] = mEntity;
        }

        // Otherwise, grow the linked list of entities at that location, the current
        // entity is the head of the chain.
        else
        {
            mEntity->mNextEntity = mWorld[(mWindowWidth * mLocation.y) + mLocation.x];
        }
    }
    else
    {
        lock_guard<mutex> lk(mRightMtx);

        // If the location is empty add the current entity to the location.
        if ((mWorld[(mWindowWidth * mLocation.y) + mLocation.x]) == nullptr)
        {
            mWorld[(mWindowWidth * mLocation.y) + mLocation.x] = mEntity;
        }

        // Otherwise, grow the linked list of entities at that location, the current
        // entity is the head of the chain.
        else
        {
            mEntity->mNextEntity = mWorld[(mWindowWidth * mLocation.y) + mLocation.x];
        }
    }

}