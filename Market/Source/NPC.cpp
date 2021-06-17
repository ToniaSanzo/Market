#pragma once
#include "PCH.h"
#include "NPC.h"


// Construct a non-initialized NPC
NPC::NPC()
{
    cout << "NPC created!\n";
}


// Display NPC to user
void NPC::render()
{
    cout << "NPC rendered!\n";
}


// Returns the EEntityType of the NPC
EEntityType NPC::getType()
{
    return EEntityType::NPC;
}