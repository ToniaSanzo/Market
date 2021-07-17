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

/**
* The different subclasses that inherit from Entity
*/
enum class EEntityType
{
    NPC,
    RUG
};


/**
* Abstract base class that represents different game entities, this class guarentees we will be able
* to safely cast to the derived base class given an Entity object.
*/
class Entity
{
private:
    static uint32_t sCreatedEntityCount;    // Number of unique Entities created.
    uint32_t mID;                           // This Entity's unique ID.
    uint32_t mCurrSubspace;                 // The subspace index the Entity's in. 
                                            // (If the mCurrSubspace is UINT32_MAX, then the
                                            //  entity has not been added to the subspace.)

public:
    /**
    * Every time an Entity is constructed increment the sCreatedEntityCount, and assign this 
    * Entity a unique ID.
    */
    Entity()
    {
        mID = ++sCreatedEntityCount;
        mCurrSubspace = UINT32_MAX;
        cout << "Abstract Base Class (Entity) constructor called [mID = " << mID << "]!\n";
    }

    /**
    *  Returns the entity type of the derived class.
    * 
    * @return EEntityType the enum of the derived subclass (i.e. Rug, NPC).
    */
    virtual EEntityType getType() = 0;

    /**
    * Get an Entity pointer of this object.
    * 
    * @return Entity* pointer to this Entity.
    */
    virtual Entity* getEntity() = 0;

    /**
    * Renders the Entity in the window.
    */
    virtual void render() = 0;

    /**
    * Set the Entity's current subspace.
    * 
    * @param aCurrSubspace - The index of the current Subspace.
    */
    void setSubspace(const uint32_t& aCurrSubspace)
    {
        mCurrSubspace = aCurrSubspace;
    }

    /**
    * Get the Entity's current subspace.
    * 
    * @return uint32_t the current subspace of the Entity.
    */
    uint32_t getSubspace()
    {
        return mCurrSubspace;
    }

    /**
    * Get the Entity's unique ID number.
    * 
    * @return The unique ID number of the Entity.
    */
    uint32_t getUniqueID()
    {
        return mID;
    }

    /**
    * Get the Entity's current location.
    * 
    * @return Vector3 the current location of the Entity.
    */
    virtual Vector3 getLocation() = 0;

};

uint32_t Entity::sCreatedEntityCount = 0;