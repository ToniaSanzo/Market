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
#include "SDLManager.h"
#include "Entity.h"
#include "Texture.h"
#include "Timer.h"


// The NPC's color
enum class EColor
{
    RED,
    GREEN,
    BLUE
};


class NPC : public Entity
{
private:
    // Represents the current state of the NPC
    ETradeState mState;

    // Number of NPCs constructed
    static int sNPCCount;

    // The index of this NPC
    int mNPCIdx;

    // Pointer to the NPCs texture, and SDL_Rect array
    Texture* mTexturePtr;
    SDL_Rect* mTextureFrames;

    // The color of the NPC
    EColor mNPCColor;

    // The current step the NPC animation is making (left, or right),
    // and the index of the frame to render to the screen
    int mCurrStep;
    int mCurrFrame;

    // The speed the NPC walks, time it takes to 
    // alternate animation speed, and time since the
    // last frame change
    float mSpeed;
    float mAnimationSpeed;
    float mCurrAnimTime;

    // The location the NPC want's to walk to, 
    // and whether we need to generate a new location to walk
    // to.
    int mTargetX, mTargetY;
    bool bNewWalkLocation;

    // The NPC's coordinates
    int mCurrX, mCurrY;

public:
    // Default initialize the NPC, the NPC will not be properly initialized until it is
    // initialized with init(..)
    NPC();

    // Deallocate resources used by the NPC
    ~NPC();

    // Properly initialize the NPC
    bool init(Texture* aTxtrPtr, SDL_Rect* aTxtrFrames);

    // Display NPC to user
    void render();

    // Update the NPC
    void update(const float& dt);
    
    // Returns the EEntityType of the NPC
    EEntityType getType();
};