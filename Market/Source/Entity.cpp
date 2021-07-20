/**
* Title: Market
* Author: Tonia Sanzo
* Date: 7/19/21
*
* Entity is a wrapper class for game entities, that guarentee we will be able to derive the
* subclass of the object
*/
#include "PCH.h"
#include "Entity.h"

uint32_t Entity::sCreatedEntityCount = 0;

/**
* Every time an Entity is constructed increment the sCreatedEntityCount, and assign this
* Entity a unique ID.
*/
Entity::Entity()
{
    mID = ++sCreatedEntityCount;
    mCurrSubspace = UINT32_MAX;
    cout << "Abstract Base Class (Entity) constructor called [mID = " << mID << "]!\n";
}


/**
* Set the Entity's current subspace.
*
* @param aCurrSubspace - The index of the current Subspace.
*/
void Entity::setSubspace(const uint32_t& aCurrSubspace)
{
    mCurrSubspace = aCurrSubspace;
}


/**
* Get the Entity's current subspace.
*
* @return uint32_t the current subspace of the Entity.
*/
uint32_t Entity::getSubspace()
{
    return mCurrSubspace;
}


/**
* Get the Entity's unique ID number.
*
* @return The unique ID number of the Entity.
*/
uint32_t Entity::getUniqueID()
{
    return mID;
}

