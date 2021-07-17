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
* Removes the Entity from the world, uses the Entity's subspace to determine which subspace
* to remove from.
*
* @param aEntity   - Entity to remove from the world.
*/
void World::removeEntity(Entity* aEntity)
{
    uint32_t col = aEntity->getSubspace() % mHorizontalTileCount;

    // Remove the Entity from the leftmost partition
    if (col < (mHorizontalTileCount / PARTITION_COUNT))
    {
        lock_guard<mutex> lk(mLeftMtx);

        mWorld[aEntity->getSubspace()]->removeEntity(aEntity);
    }

    // Remove the Entity from the center partition
    else if (col < (2 * mHorizontalTileCount) / PARTITION_COUNT)
    {
        lock_guard<mutex> lk(mCenterMtx);

        mWorld[aEntity->getSubspace()]->removeEntity(aEntity);
    }

    // Remove the Entity from the rightmost partition
    else
    {
        lock_guard<mutex> lk(mRightMtx);

        mWorld[aEntity->getSubspace()]->removeEntity(aEntity);
    }
}


/**
* Adds the Entity to the world, uses the Entity's subspace to determine which subspace to add to.
*
* @param aEntity - Entity to add to the world.
*/
void World::addEntity(Entity* aEntity)
{
    uint32_t col = aEntity->getSubspace() % mHorizontalTileCount;

    // Add the Entity to the leftmost partition
    if (col < (mHorizontalTileCount / PARTITION_COUNT))
    {
        lock_guard<mutex> lk(mLeftMtx);

        mWorld[aEntity->getSubspace()]->addEntity(aEntity);
    }

    // Add the Entity to the center partition
    else if (col < (2 * mHorizontalTileCount) / PARTITION_COUNT)
    {
        lock_guard<mutex> lk(mCenterMtx);

        mWorld[aEntity->getSubspace()]->addEntity(aEntity);
    }
    // Remove the Entity from the rightmost partition
    else
    {
        lock_guard<mutex> lk(mRightMtx);

        mWorld[aEntity->getSubspace()]->addEntity(aEntity);
    }
}


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
void World::placeEntity(Entity* aEntity, const Vector3& mNewLocation)
{
    // the vector index of the given location
    uint32_t newRow      = mNewLocation.y / mRenderTileLength;
    uint32_t newCol      = mNewLocation.x / mRenderTileLength;
    uint32_t newSubspace = (newRow * mHorizontalTileCount) + newCol;

    // Update the subspaces only if the entity moved to a different subspace
    if (newSubspace != aEntity->getSubspace())
    {
        cout << "Moving Entity from subspace [" << aEntity->getSubspace() << "] to ["
            << newSubspace << "]\n";
        if (newSubspace > mWorld.size())
        {
            cout << "[Tonia Sanzo] INDEX OUT OF RANGE EXCEPTION!\n";
            exit(1);
        }

        // Checks if the Entity has been placed in the world, if it has it removes the Entity
        if (aEntity->getSubspace() != UINT32_MAX)
        {
            removeEntity(aEntity);
        }

        // Set the entity's new subspace and add the Entity to the world
        aEntity->setSubspace(newSubspace);
        addEntity(aEntity);
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


/**
* Adds an Entity to the subspace.
*
* @param aEntity - reference to the entity being added to the subspace.
*/
void Subspace::addEntity(Entity* aEntity)
{
    mEntities.push_back(aEntity);
}


/**
* Remove target Entity from the subspace, if the Entity is present.
*
* @param aEntity - reference to the entity being removed from the subspace.
*/
void Subspace::removeEntity(Entity* aEntity)
{
    auto itr = mEntities.begin();
    for ( ; itr != mEntities.end(); ++itr)
    {
        if ((*itr)->getUniqueID() == aEntity->getUniqueID())
        {
            mEntities.erase(itr);
        }
    }
}


/**
* Sorts the entities within the subspace based on the entities locations.
* 
* @param low - Starting index.
* @param high - Ending index.
*/
void Subspace::quickSort(uint32_t low, uint32_t high)
{

}

/**
* Moves all the values higher than the pivot to the right of the pivot
*
* @param low - Starting index.
* @param high - Ending index.
* @return uint32_t the index of the pivot position.
*/
uint32_t Subspace::partition(uint32_t low, uint32_t high)
{
    Entity* pivot = mEntities[high];
    uint32_t i = low;

    for (uint32_t j = low; j <= high; ++j)
    {
        if (mEntities[j]->getLocation().y < pivot->getLocation().y)
        {
            Finish implementing QUICKSORT here
        }

    }
}
