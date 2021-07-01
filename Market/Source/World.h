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


class World
{
private:
    vector<Entity*> mWorld;
    
public:
    // Default constructor
    World();

    // Initialize the mWorld vector
    bool init();
};