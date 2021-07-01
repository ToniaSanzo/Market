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


// Default constructor
World::World()
{}

// Initialize the mWorld vector
bool World::init()
{
    if ((SDLManager::mWindowHeight * SDLManager::mWindowWidth) == 0)
    {
        cout << "Failed to initialize the World!\n";
        return false;
    }
    else
    {
        // Initialize window here
        mWorld.resize((SDLManager::mWindowHeight * SDLManager::mWindowWidth), nullptr);
        cout << "World size: " << mWorld.size() << "\n";
    }

    return true;

}
