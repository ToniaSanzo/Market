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


// This class is a simple class that shares data with multiple threads
class Rug : public Entity
{
private:
    // Represents the current state of the rug, the state of the rug will be accessed by multiple threads,
    // so we added a mutex lock
    ETradeState mState;
    mutex mStateMtx;

    // Number of rugs constructed
    static int rugCount;

    // The index of this rug
    int rugIdx;

public:
    // Default initialize the rug, the rug will not be properly initialized until it is
    // initialized with init
    Rug();

    // Deallocate resources used by the  rug
    ~Rug();

    // Properly initialize the rug 
    bool init(SDLManager* aSDL);

    // Draw the rug to the screen
    void render();

    // Update the state of the rug
    void updateState(const ETradeState& aState);

    // Returns the EEntityType of the rug
    EEntityType getType();
};