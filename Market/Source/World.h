/**
* Title: Market
* Author: Tonia Sanzo
* Date: 6/15/21
*
* World is 2D array that represents the game world and the positions of the rugs and npcs 
* within that game world.
*/
#pragma once
#include "PCH.h"
#include "Entity.h"


// The world is partitioned into 3 equal partitions
enum class EWorldPartition
{
    LEFT,
    CENTER,
    RIGHT
};


class World
{
private:

    vector<Subspace*> mWorld;

    uint32_t mWindowWidth;
    uint32_t mWindowHeight;

    mutex mLeftMtx;
    mutex mCenterMtx;
    mutex mRightMtx;

    float mRenderTileLength;
    uint32_t mHorizontalTileCount;
    uint32_t mVerticalTileCount;


    /**
    * Removes the Entity from the world, uses the Entity's subspace to determine which subspace
    * to remove from.
    * 
    * @param aEntity   - Entity to remove from the world.
    */
    void removeEntity(Entity* aEntity);

    /**
    * Adds the Entity to the world, uses the Entity's subspace to determine which subspace to add to.
    * 
    * @param aEntity   - Entity to add to the world.
    */
    void addEntity(Entity* aEntity);

    
public:
    /**
    * Default Constructor, default iniatialize every member variable
    */
    World();

    /**
    * Properly initialize the world, this uses the SDLManager's window dimensions to
    * create a vector that has an index for each pixel in the window.
    */
    bool init();

    /**
    * Place an Entity into the game world at a certain location
    * 
    * @param Entity* - reference to the Entity being added to the world
    * @param mLocation - location to add the Entity to
    */
    void placeEntity(Entity* mEntity, const Vector3& mLocation);


    /**
    * Renders every entity in a partition of the world, this is designed to be done concurrently
    * 
    * @param aPartition - The partition to render
    */
    void render(const EWorldPartition& aPartition);


    /**
    * Renders the Entity chain at the given index location
    * 
    * @param aIndex - The mWorld vector index of the location
    */
    void renderLocation(const size_t& aIndex);
};


class Subspace
{
private:
    // World can access private/protected members of Subspace
    friend class World; 

    // Vector of entites within the subspace
    vector<Entity*> mEntities;

    /**
    * Adds an Entity to the subspace. (Warning! does not order the subspace based on the entities coordinate)
    *
    * @param aEntity - reference to the entity being added to the subspace.
    */
    void addEntity(Entity* aEntity);

    /**
    * Remove target Entity from the subspace
    * 
    * @param aEntity - reference to the entity being removed from the subspace
    */
    void removeEntity(Entity* aEntity);

    /**
    * Reorders the entities within the subspace based on the entities locations
    */
    void reorder();
};