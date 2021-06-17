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
#include "Entity.h"


class NPC : public Entity
{
public:
    // Construct a non-initialized NPC
    NPC();

    // Display NPC to user
    void render();
    
    // Returns the EEntityType of the NPC
    EEntityType getType();
};