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
#include "Rug.h"
#include "NPC.h"
#include "SDLManager.h"


/**
* Removes the Entity from the world, uses the Entity's subspace to determine which subspace
* to remove from.
* @param aEntity - Entity to remove from the world.
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
    mHorizontalTileCount = 9.f;
    mVerticalTileCount   = 0;
}


/**
* Deallocate the subspaces used in this world
*/
World::~World()
{
    for (Subspace* subspace : mWorld)
    {
        delete subspace;
    }
    mWorld.clear();
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
        for(uint32_t i = 0; i < static_cast<size_t>(mHorizontalTileCount) * static_cast<size_t>(mVerticalTileCount); ++i)
        {
            mWorld.push_back(new Subspace());
        }

    }

    return success;
}


/**
* Add an entity to the world at a certain location.
* @param Entity* - reference to the entity being added to the world
*/
void World::placeEntity(Entity* aEntity)
{
    // the vector index of the given location
    uint32_t newRow      = aEntity->getLocation().y / mRenderTileLength;
    uint32_t newCol      = aEntity->getLocation().x / mRenderTileLength;
    uint32_t newSubspace = (newRow * mHorizontalTileCount) + newCol;

    // Update the subspaces only if the entity moved to a different subspace
    if (newSubspace != aEntity->getSubspace())
    {
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
* Each Subspace's Entity chain in the given partition will be organized based on
* the Entitiy's y-coordinate.
* @param aPartition - The partition to order.
*/
void World::orderWorld(const EWorldPartition& aPartition)
{
    switch (aPartition)
    {
    // Order the leftmost partition
    case EWorldPartition::LEFT:
        for (uint32_t col = 0; col < (mHorizontalTileCount / PARTITION_COUNT); ++col)
        {
            for (uint32_t row = 0; row < mVerticalTileCount; ++row)
            {
                lock_guard<mutex> lock(mLeftMtx);
                mWorld[(static_cast<size_t>(mHorizontalTileCount) * row) + col]->order();
                mWorld[(static_cast<size_t>(mHorizontalTileCount) * row) + col]->trade();
            }
        }
        break;

    // Order the center partition
    case EWorldPartition::CENTER:
        for (uint32_t col = mHorizontalTileCount / 3.f; col < (2 * mHorizontalTileCount) / PARTITION_COUNT; ++col)
        {
            for (uint32_t row = 0; row < mVerticalTileCount; ++row)
            {
                lock_guard<mutex> lock(mCenterMtx);
                mWorld[(static_cast<size_t>(mHorizontalTileCount) * row) + col]->order();
                mWorld[(static_cast<size_t>(mHorizontalTileCount) * row) + col]->trade();
            }
        }
        break;

    // Order the rightmost partition
    case EWorldPartition::RIGHT:
        for (uint32_t col = ((2 * mHorizontalTileCount) / 3.f); col < mHorizontalTileCount; ++col)
        {
            for (uint32_t row = 0; row < mVerticalTileCount; ++row)
            {
                lock_guard<mutex> lock(mRightMtx);
                mWorld[(static_cast<size_t>(mHorizontalTileCount) * row) + col]->order();
                mWorld[(static_cast<size_t>(mHorizontalTileCount) * row) + col]->trade();
            }
        }
        break;
    }
}


/**
* Renders every entity in a partition of the world, this is designed to be done concurrently
* @param aPartition - The partition to render
*/
void World::render(const EWorldPartition& aPartition)
{
    switch (aPartition)
    {
    // Render the leftmost partition
    case EWorldPartition::LEFT:
        for (uint32_t col = 0; col < (mHorizontalTileCount / 3.f); ++col)
        {
            for (uint32_t row = 0; row < mVerticalTileCount; ++row)
            {
                size_t currIndex = (static_cast<size_t>(mHorizontalTileCount) * row) + col;
                lock_guard<mutex> lock(mLeftMtx);

                // If the current location has an entity render the location
                if (mWorld[currIndex])
                {
                    renderLocation(currIndex);
                }
            }
        }
        break;

    // Render the center partition
    case EWorldPartition::CENTER:
        for (uint32_t col = mHorizontalTileCount / 3.f; col < ((2 * mHorizontalTileCount) / 3.f); ++col)
        {
            for (uint32_t row = 0; row < mVerticalTileCount; ++row)
            {
                lock_guard<mutex> lock(mCenterMtx);
                size_t currIndex = (static_cast<size_t>(mHorizontalTileCount) * row) + col;

                // If the current location has an entity render the location
                if (mWorld[currIndex])
                {
                    renderLocation(currIndex);
                }
            }
        }
        break;

    // Render the rightmost partition
    case EWorldPartition::RIGHT:
        for (uint32_t col = ((2 * mHorizontalTileCount) / 3.f); col < mHorizontalTileCount; ++col)
        {
            for (uint32_t row = 0; row < mVerticalTileCount; ++row)
            {
                lock_guard<mutex> lock(mRightMtx);
                size_t currIndex = (static_cast<size_t>(mHorizontalTileCount) * row) + col;

                // If the current location has an entity render the location
                if (mWorld[currIndex])
                {
                    renderLocation(currIndex);
                }
            }
        }
        break;
    }
}


/**
* Renders the Entity chain for the given subspace
* @param aIndex - The mWorld index of the subspace to render
*/
void World::renderLocation(const size_t& aIndex)
{
    Subspace* targetSubspace = mWorld[aIndex];

    for (Entity* entity : targetSubspace->mEntities)
    {
        entity->render();
    }
}


/**
* Constructor
*/
Subspace::Subspace()
{}


/**
* Destructor
*/
Subspace::~Subspace()
{
    mEntities.clear();
}


/**
* Adds an Entity to the subspace.
* @param aEntity - reference to the entity being added to the subspace.
*/
void Subspace::addEntity(Entity* aEntity)
{
    mEntities.push_back(aEntity);
}


/**
* Remove target Entity from the subspace, if the Entity is present.
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
            return;
        }
    }
}


/**
* Order the Subspace.
*/
void Subspace::order()
{
    // Only sort Subspaces with more than one Entity
    if (mEntities.size() > 1)
    {
        quickSort(0, mEntities.size() - 1);
    }
}


/**
* Sorts the entities within the subspace based on the entities locations.
* @param low - Starting index.
* @param high - Ending index.
*/
void Subspace::quickSort(uint32_t low, uint32_t high)
{
    if (low < high)
    {
        uint32_t pivot = partition(low, high);
        quickSort(low, pivot);
        quickSort(pivot + 1, high);
    }
}


/**
* All the values less then the center value are moved lower of the 
* center value, and the values greater than the center value are moved upper of the 
* center value.
* @param low - Starting index.
* @param high - Ending index.
* @return uint32_t the index where the left index is equal to or greater then the 
*                  right index.
*/
uint32_t Subspace::partition(uint32_t low, uint32_t high)
{
    if (((high + low) / 2) == mEntities.size())
    {
        cout << "w!\n";
    }
    Entity* pivot = mEntities[(high + low) / 2];
    
    uint32_t lowerIndex  = low - 1;
    uint32_t upperIndex = high + 1;

    while (true)
    {
        do
        {
            ++lowerIndex;
        } while (mEntities[lowerIndex]->getLocation().y < pivot->getLocation().y);
        
        do
        {
            --upperIndex;
        } while (mEntities[upperIndex]->getLocation().y > pivot->getLocation().y);
        
        if (lowerIndex >= upperIndex)
        {
            return upperIndex;
        }
        swap(lowerIndex, upperIndex);
    }
}


/**
* Swap the Entity's given by the index numbers.
* @param aIndex1 - Index of the first Entity to be swapped.
* @param aIndex2 - Index of the second Entity to be swapped.
*/
void Subspace::swap(uint32_t aIndex1, uint32_t aIndex2)
{
    Entity* tempEntity = mEntities[aIndex1];
    mEntities[aIndex1] = mEntities[aIndex2];
    mEntities[aIndex2] = tempEntity;
}


/**
* Trade objects when NPC's overlap Rug's in a single subspace.
*/
void Subspace::trade()
{
    // Iterators to the first Entity, and one past the last Entity in the mEntities vector
    vector<Entity*>::iterator begin = mEntities.begin();
    vector<Entity*>::iterator end   = mEntities.end();
    
    // Iterators above, below, and on the current Entity
    vector<Entity*>::iterator currEntity;
    vector<Entity*>::iterator entityAbove;
    vector<Entity*>::iterator entityBelow;

    // Whether we can continue looking above/below for Entity's overlapping the currEntity
    bool lookAbove = true;
    bool lookBelow = true;

    // Iterate through the subspace
    for (currEntity = begin; currEntity != end; ++currEntity)
    {
        // If currEntity is a Rug, check for overlaps
        if ((*currEntity)->getType() == EEntityType::RUG)
        {
            Rug* tempRug = static_cast<Rug*>(*currEntity);

            // Confirm the currEntity can trade
            if (tempRug->canTrade())
            {
                // look for the next entity that is able to trade with the currEntity
                entityAbove = entityBelow = currEntity;

                while (lookAbove || lookBelow)
                {
                    if (lookAbove)
                    {
                        // Stop looking at entityAbove when entityAbove's location moves out of range of currEntity
                        if (((*entityAbove)->getLocation().y - tempRug->getLocation().y) > TRADE_RADIUS)
                        {
                            lookAbove = false;
                        }
                        else
                        {
                            // Confirm the entityAbove is a NPC, and in a different trade state
                            if (((*entityAbove)->getType() == EEntityType::NPC) && ((*entityAbove)->getTradeState() != tempRug->getTradeState()))
                            {
                                // Cast the entityAbove to a NPC, then check if the tempNPC moved within the trade 
                                // radius of the Rug
                                NPC* tempNPC = static_cast<NPC*>(*entityAbove);
                                if (MATH::doesLineSegmentOverlapCircle(tempNPC->getPrevLocation(), tempNPC->getLocation(), tempRug->getLocation(), TRADE_RADIUS))
                                {
                                    if (!tempNPC->getPreviousOverlappingRug())
                                    {
                                        // Have the entity's trade
                                        ETradeState tempTradeState = tempRug->getTradeState();
                                        tempRug->setTradeState((*entityAbove)->getTradeState());
                                        (*entityAbove)->setTradeState(tempTradeState);
                                        lookAbove = lookBelow = true;
                                    }

                                    tempNPC->setOverlappingRug(true);
                                    break;
                                }
                            }

                            if (entityAbove == begin)
                            {
                                lookAbove = false;
                            }
                            else
                            {
                                --entityAbove;
                            }
                        }
                    }

                    if (lookBelow)
                    {
                        // Stop looking at entityBelow when entityBelow's location moves out of range of currEntity
                        if (((*currEntity)->getLocation().y - (*entityBelow)->getLocation().y) > TRADE_RADIUS)
                        {
                            lookBelow = false; 
                        }
                        else
                        {
                            // Confirm the entityBelow is a NPC, and in a different trade state
                            if (((*entityBelow)->getType() == EEntityType::NPC) && ((*entityBelow)->getTradeState() != tempRug->getTradeState()))
                            {
                                // Cast the entityBelow to a NPC, then check if the tempNPC moved within the trade 
                                // radius of the Rug
                                NPC* tempNPC = static_cast<NPC*>(*entityBelow);
                                if (MATH::doesLineSegmentOverlapCircle(tempNPC->getPrevLocation(), tempNPC->getLocation(), tempRug->getLocation(), TRADE_RADIUS))
                                {
                                    if (!tempNPC->getPreviousOverlappingRug())
                                    {
                                        // Have the entity's trade
                                        ETradeState tempTradeState = tempRug->getTradeState();
                                        tempRug->setTradeState((*entityBelow)->getTradeState());
                                        (*entityBelow)->setTradeState(tempTradeState);
                                        lookAbove = lookBelow = true;
                                    }

                                    tempNPC->setOverlappingRug(true);
                                    break;
                                }
                            }

                            if (++entityBelow == end)
                            {
                                lookBelow = false;
                            }
                        }
                    }
                }
            }
        }

        // If currEntity is an NPC, double check for overlaps
        if ((*currEntity)->getType() == EEntityType::NPC)
        {
            NPC* tempNPC = static_cast<NPC*>(*currEntity);

            // If the currEntity was previously overlapping the rug, and not just set to overlap, confirm if it
            // is still overlapping or not.
            if (tempNPC->getPreviousOverlappingRug() && !tempNPC->getCurrentOverlappingRug())
            {
                // look for the next entity that is able to trade with the currEntity
                entityAbove = entityBelow = currEntity;

                while (lookAbove || lookBelow)
                {
                    if (lookAbove)
                    {
                        // Stop looking at entityAbove when entityAbove's location moves out of range of currEntity
                        if (((*entityAbove)->getLocation().y - tempNPC->getLocation().y) > TRADE_RADIUS)
                        {
                            lookAbove = false;
                        }
                        else
                        {
                            // Confirm the entityAbove is a Rug, and in a different trade state
                            if (((*entityAbove)->getType() == EEntityType::RUG) && ((*entityAbove)->getTradeState() != tempNPC->getTradeState()))
                            {
                                // Cast the entityAbove to a Rug, then check if the tempNPC overlaps the trade 
                                // radius of the Rug
                                Rug* tempRug = static_cast<Rug*>(*entityAbove);
                                if (MATH::doesLineSegmentOverlapCircle(tempNPC->getPrevLocation(), tempNPC->getLocation(), tempRug->getLocation(), TRADE_RADIUS))
                                {
                                   tempNPC->setOverlappingRug(true);
                                    break;
                                }
                            }

                            if (entityAbove == begin)
                            {
                                lookAbove = false;
                            }
                            else
                            {
                                --entityAbove;
                            }
                        }
                    }

                    if (lookBelow)
                    {
                        // Stop looking at entityBelow when entityBelow's location moves out of range of currEntity
                        if (((tempNPC)->getLocation().y - (*entityBelow)->getLocation().y) > TRADE_RADIUS)
                        {
                            lookBelow = false;
                        }
                        else
                        {
                            // Confirm the entityBelow is a Rug, and in a different trade state
                            if (((*entityBelow)->getType() == EEntityType::RUG) && ((*entityBelow)->getTradeState() != tempNPC->getTradeState()))
                            {
                                // Cast the entityBelow to a Rug, then check if the NPC overlaps the trade 
                                // radius of the Rug
                                Rug* tempRug = static_cast<Rug*>(*entityBelow);
                                if (MATH::doesLineSegmentOverlapCircle(tempNPC->getPrevLocation(), tempNPC->getLocation(), tempRug->getLocation(), TRADE_RADIUS))
                                {
                                    tempNPC->setOverlappingRug(true);
                                    break;
                                }
                            }

                            if (++entityBelow == end)
                            {
                                lookBelow = false;
                            }
                        }
                    }
                }
            }
        }
    }
}
