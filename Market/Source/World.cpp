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
        mWorld.resize((mWindowWidth * mWindowHeight), nullptr);
        cout << "World size: " << mWorld.size() << "\n";
    }

    return success;

}


/**
* Add an entity to the world at a certain location
*
* @param Entity* - reference to the entity being added to the world
* @param mLocation - location to move the entity to
*/
void World::addEntity(Entity* mEntity, const Vector3& mLocation)
{
    EWorldPartition partition;

    // Determine which partition the entity is being added too,
    // Lock that partition and add the element to it
    if (mLocation.x < mWindowWidth / 3)
    {

    }
    else if (mLocation.x > (2 * mWindowWidth) / 3)
    {
        
    }
    else
    {
        YOU LEFT OFF HERE MAKE SURE TO LOCK THE PARTITION AND add the entityand add the entity at the location, if the location has another entityadd it to the front
            oof the pixel linked list
    }

}