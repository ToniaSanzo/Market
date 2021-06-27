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
    static uint16_t sNPCCount;

    // The index of this NPC
    uint16_t mNPCIdx;

    // Pointer to the NPCs texture, and SDL_Rect array
    Texture* mTexturePtr;
    SDL_Rect* mTextureFrames;

    // The color of the NPC
    EColor mNPCColor;

    // The current step the NPC animation is making (left, or right),
    // and the index of the frame to render to the screen
    uint16_t mCurrStep;
    uint16_t mCurrFrame;

    // The speed the NPC walks, time it takes to 
    // alternate animation speed, and time since the
    // last frame change
    float mSpeed;
    float mAnimationSpeed;
    float mCurrAnimTime;

    // The location the NPC want's to walk to, the direction
    // the NPC has to walk to get there 
    // and whether we need to generate a new location to walk
    // to.
    float mTargetX, mTargetY;
    Vector3 mDirection;
    bool bNewWalkLocation;

    // The NPC's coordinates
    float mCurrX, mCurrY;

    // Generate new walk location of the NPC  
    void setNewWalkLocation(const float& aRandomX, const float& aRandomY);

    // Set NPC's direction
    void setDirection();

public:
    // Default initialize the NPC, the NPC will not be properly initialized until it is
    // initialized with init(..)
    NPC();

    // Deallocate resources used by the NPC
    ~NPC();

    // Properly initialize the NPC
    bool init(Texture* aTxtrPtr, SDL_Rect aTxtrFrames[]);

    // Display NPC to user
    void render();

    // Update the NPC
    void update(const float& dt, const float& aRandomX, const float& aRandomY);
    
    // Returns the EEntityType of the NPC
    EEntityType getType();
    
    // Set NPC's speed, and animation speed giving a value between 0 and 1, clamped between 
    // the (MIN_SPEED | MIN_ANIMATION_SPEED) and (MAX_SPEED | MAX_ANIMATION_SPEED)
    void setSpeed(const float&);
};