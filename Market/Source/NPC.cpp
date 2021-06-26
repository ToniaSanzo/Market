#pragma once
#include "PCH.h"
#include "NPC.h"
#include "Game.h"


// Number of NPCs constructed
int NPC::sNPCCount = 0;


// Construct a non-initialized NPC
NPC::NPC()
{
    mNPCIdx = ++sNPCCount;

    // Set the NPC's member variables to random values
    mState = static_cast<ETradeState>(rand() % 3);
    mNPCColor = static_cast<EColor>(rand() % 3);
    mCurrStep = rand() % 2;
    mCurrFrame = static_cast<int>(mNPCColor) * NPC_FRAMES_COLS;
    mCurrFrame += static_cast<int>(mState) * NPC_TRADE_FRAMES;
    mCurrFrame += mCurrStep;

    // Generate a random location for the NPC to spawn
    mCurrX = rand() % SDLManager::mWindowWidth;
    mCurrY = rand() % SDLManager::mWindowHeight;

    // Generate a random location to walk too
    mTargetX = rand() % SDLManager::mWindowWidth;
    mTargetY = rand() % SDLManager::mWindowHeight;
    bNewWalkLocation = false;

    // Set random movement speed of the NPC
    mSpeed = (rand() % static_cast<int>(MAX_SPEED - MIN_SPEED)) + MIN_SPEED;
    mAnimationSpeed = ((((MAX_ANIMATION_SPEED + MAX_SPEED) - mSpeed) * .01f) * 3.5f) + .15f;
    mCurrAnimTime = 0;

    mTexturePtr = nullptr;
    mTextureFrames = nullptr;
}


// allocate resources used by the NPC, do not explicitly delete the references
// as they are shared between each rug instance and deallocated elsewhere
NPC::~NPC()
{
    if (mTexturePtr)
    {
        mTexturePtr = nullptr;
    }

    if (mTextureFrames)
    {
        mTextureFrames = nullptr;
    }
}


bool NPC::init(Texture* aTxtrPtr, SDL_Rect aTxtrFrames[])
{
    // Initialization success flag
    bool success = true;

    if (!aTxtrPtr)
    {
        cout << "Failed, valid Texture pointer is required in Rug::init(Texture*, SDL_Rect*)\n";
        success = false;
    }
    else
    {
        mTexturePtr = aTxtrPtr;
    }

    if (!aTxtrFrames)
    {
        cout << "Failed, valid Texture pointer is required in Rug::init(Texture*, SDL_Rect*)\n";
        success = false;
    }
    else
    {
        mTextureFrames = aTxtrFrames;
    }

    return success;
}


// NPC's have different speeds
void NPC::update(const float& dt, const float& aRandomX, const float& aRandomY)
{
    // Whether the trade state changed and we need to update the current frame
    bool bUpdateFrame = false;
    
    cout << "mTargetLocation{x: " << mTargetX << ", y: " << mTargetY << "} mCurrentLocation{x: " << mCurrX << ", y: " << mCurrY << "}\n";

    // Generate a new target location to walk to based on if the target location was reached
    if (bNewWalkLocation)
    {
        mTargetX = aRandomX * SDLManager::mWindowWidth;
        mTargetY = aRandomY * SDLManager::mWindowHeight;
        bNewWalkLocation = false;
    }
    // Otherwise, continue walking towards the target location
    else
    {
        float changeInX = mTargetX - mCurrX;
        float changeInY = mTargetY - mCurrY;

        // If NPC is in range of new location set the bNewWalkLocation flag
        if ((changeInX * changeInX) + (changeInY * changeInY) < (NPC_TARGET_LOCATION_RANGE * NPC_TARGET_LOCATION_RANGE))
        {
            bNewWalkLocation = true;
        }
        // Otherwise, move closer to the target location
        else
        {
            mCurrX += changeInX * (dt * mSpeed);
            mCurrY += changeInY * (dt * mSpeed);
        }
    }

    // Update and change trade state before setting the current frame

    // Set the current frame
    mCurrAnimTime += dt;
    if (mCurrAnimTime > mAnimationSpeed)
    {
        mCurrStep = ++mCurrStep % NPC_TRADE_FRAMES;
        mCurrAnimTime = 0;
        bUpdateFrame = true;
    }
    
    if (bUpdateFrame)
    {
        mCurrFrame = static_cast<int>(mNPCColor) * NPC_FRAMES_COLS;
        mCurrFrame += static_cast<int>(mState) * NPC_TRADE_FRAMES;
        mCurrFrame += mCurrStep;
        bUpdateFrame = false;
    }
}


// Display NPC to user
void NPC::render()
{
    mTexturePtr->render(mCurrX, mCurrY, &mTextureFrames[static_cast<int>(mCurrFrame)]);
}


// Returns the EEntityType of the NPC
EEntityType NPC::getType()
{
    return EEntityType::NPC;
}