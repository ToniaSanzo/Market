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
    mWindowWidth         = 0;
    mWindowHeight        = 0;
    mRenderTileLength    = 0;
    mHorizontalTileCount = 12.f;
    mVerticalTileCount   = 0;
}


/**
* Initialize the World to match the dimensions of the map
*/
bool World::init()
{
    bool success = true;

    mWindowWidth  = SDLManager::mWindowWidth + 1;
    mWindowHeight = SDLManager::mWindowHeight + 1;

    // Confirm the SDLManager window is initialized
    if ((mWindowHeight * mWindowWidth) == 0)
    {
        cout << "Failed to initialize the World!\n";
        success = false;
    }
    else
    {
        // Because the world is made up of square render tiles, determine the dimensions
        // and number of tiles needed
        mRenderTileLength = mWindowWidth / static_cast<float>(mHorizontalTileCount);
        mVerticalTileCount = (mWindowHeight / mRenderTileLength) + 1;

        // mWorld is a "1D" array that represents a "2D" world
        mWorld.resize(static_cast<size_t>(mHorizontalTileCount) * static_cast<size_t>(mVerticalTileCount), nullptr);

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
    cout << "Adding Entity at location: [" << locationIndex << "]\n";
    if (locationIndex > mWorld.size())
    {
        exit(1);
    }

    // Determine which partition the entity is being added to,
    // lock that partition and add the element to it.
    if (mLocation.x < mWindowWidth / PARTITION_COUNT)
    {
        lock_guard<mutex> lk(mLeftMtx);
        
        // If the location is empty add the current entity to the location.
        if (mWorld[locationIndex] == nullptr)
        {
            mWorld[locationIndex] = aEntity;
        }

        // Otherwise, grow the linked list of entities at that location, the current
        // entity is the head of the chain.
        else
        {
            aEntity->mNextEntity = mWorld[locationIndex];
            mWorld[locationIndex] = aEntity;
        }
    }
    else if (mLocation.x > (2 * mWindowWidth) / PARTITION_COUNT)
    {
        lock_guard<mutex> lk(mCenterMtx);

        // If the location is empty add the current entity to the location.
        if (mWorld[locationIndex] == nullptr)
        {
            mWorld[locationIndex] = aEntity;
        }

        // Otherwise, grow the linked list of entities at that location, the current
        // entity is the head of the chain.
        else
        {
            aEntity->mNextEntity = mWorld[locationIndex];
            mWorld[locationIndex] = aEntity;
        }
    }
    else
    {
        lock_guard<mutex> lk(mRightMtx);

        // If the location is empty add the current entity to the location.
        if (mWorld[locationIndex] == nullptr)
        {
            mWorld[locationIndex] = aEntity;
        }

        // Otherwise, grow the linked list of entities at that location, the current
        // entity is the head of the chain.
        else
        {
            aEntity->mNextEntity = mWorld[locationIndex];
            mWorld[locationIndex] = aEntity;
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
        exit(1);
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


/**
* Renders every entity in a partition of the world, this is designed to be done concurrently
*
* @param aPartition - The partition to render
*/
void World::render(const EWorldPartition& aPartition)
{
    switch (aPartition)
    {
    // Render the leftmost partition
    case EWorldPartition::LEFT:
        for (uint32_t col = 0; col < (SDLManager::mWindowWidth / 3.f); ++col)
        {
            for (uint32_t row = 0; row < SDLManager::mWindowHeight; ++row)
            {
                size_t currIndex = (static_cast<size_t>(mWindowWidth) * row) + col;

                // If the current location has an entity render the location
                if (mWorld[currIndex])
                {
                    lock_guard<mutex> lock(mLeftMtx);
                    renderLocation(currIndex);
                }
            }
        }
        break;

    // Render the rightmost partition
    case EWorldPartition::CENTER:
        for (uint32_t col = SDLManager::mWindowWidth / 3.f; col < ((2 * SDLManager::mWindowWidth) / 3.f); ++col)
        {
            for (uint32_t row = 0; row < SDLManager::mWindowHeight; ++row)
            {
                size_t currIndex = (static_cast<size_t>(mWindowWidth) * row) + col;

                // If the current location has an entity render the location
                if (mWorld[currIndex])
                {
                    lock_guard<mutex> lock(mCenterMtx);
                    renderLocation(currIndex);
                }
            }
        }
        break;

    case EWorldPartition::RIGHT:
        for (uint32_t col = ((2 * SDLManager::mWindowWidth) / 3.f); col < SDLManager::mWindowWidth; ++col)
        {
            for (uint32_t row = 0; row < SDLManager::mWindowHeight; ++row)
            {
                size_t currIndex = (static_cast<size_t>(mWindowWidth) * row) + col;

                // If the current location has an entity render the location
                if (mWorld[currIndex])
                {
                    lock_guard<mutex> lock(mRightMtx);
                    renderLocation(currIndex);
                }
            }
        }
        break;
    }
}


/**
* Renders the Entity chain at the given index location
*
* @param aIndex - The mWorld vector index of the location
*/
void World::renderLocation(const size_t& aIndex)
{
    Entity* currEntity = mWorld[aIndex];

    while (currEntity)
    {
        if (currEntity->getType() == EEntityType::RUG)
        {
            return;
        }

        currEntity->render();
        currEntity = currEntity->mNextEntity;
    }
}