/**
* Title: Market
* Author: Tonia Sanzo
* Date: 6/11/21
*
* This is a rug, it uses a mutex to share data between multiple threads
*/
#pragma once
#include "PCH.h"
#include "SDLManager.h"
#include "Entity.h"
#include "Texture.h"
class World;


// This class is a simple class that shares data with multiple threads
class Rug : public Entity
{
private:
    // Represents the current state of the rug, the state of the rug will be accessed by
    //  multiple threads, so we added a mutex lock
    atomic<uint16_t> mState;
   
    // Pointer to the rugs Texture, and SDL_Rect array
    Texture* mTexturePtr;
    SDL_Rect* mTextureFrames;

    // The rugs coordinates
    Vector mLocation;

    // The amount of time before the Rug is able to be traded
    atomic<float> mTimeToTrade;
public:
    // Default initialize the rug, the rug will not be properly initialized until it is
    // initialized with init(..)
    Rug();

    // Deallocate resources used by the rug
    ~Rug();

    // Properly initialize the rug 
    bool init(Texture* aTxtrPtr, SDL_Rect aTxtrFrames[], World& aWorld);

    /**
    * update the rug
    * 
    * @param dt - time passed since last time update was called.
    */
    void update(const float& dt);

    // Only draws the rugs trade item to the screen
    void render();

    // Draw the full rug entity to the screen
    void renderFull();

    /**
    * Set this Rug's current trade state to the argument trade state.
    *
    * @param aNewTradeState - The trade state to set the Rug to.
    */
    void setTradeState(const ETradeState& aState);

    /**
    * Get this Rug's current trade state.
    *
    * @return ETradeState The trade state of the Rug.
    */
    ETradeState getTradeState();

    // Returns the EEntityType of the rug
    EEntityType getType();

    /**
    * Get's this Rug as an Entity pointer.
    * 
    * @return Entity* returns the rug as an Entity pointer.
    */
    Entity* getEntity();

    /**
    * Get the Rug's current location.
    *
    * @return Vector the current location of the Entity.
    */
    Vector getLocation();

    /**
    * Whether the current can make a trade or not
    * 
    * @return bool True the rug can trade, otherwise false
    */
    bool canTrade();
};