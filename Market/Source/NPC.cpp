#pragma once
#include "PCH.h"
#include "NPC.h"


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
    x = rand() % WINDOW_WIDTH;
    y = rand() % WINDOW_HEIGHT;

    // Set random movement speed of the NPC
    mSpeed = (rand() % static_cast<int>(MAX_SPEED - MIN_SPEED)) + MIN_SPEED;
    mAnimationSpeed = (MAX_ANIMATION_SPEED + MAX_SPEED) - mSpeed;
    cout << "NPC[" << mNPCIdx << "]: mSpeed is [" << mSpeed << "]pxl/sec and mAnimationSpeed is [" << mAnimationSpeed << "]\n";
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
void NPC::update(const float& dt)
{
    // Whether the trade state changed and we need to update the current frame
    bool bUpdateFrame = false;
    
    // Update and change trade state before setting the current frame

    // Set the current frame
    mCurrAnimTime += dt;
    if (mCurrAnimTime > mAnimationSpeed)
    {
        mCurrStep = ++mCurrStep % NPC_TRADE_FRAMES;
        bUpdateFrame = true;
    }
    
    else if (bUpdateFrame)
    {
        mCurrFrame = static_cast<int>(mNPCColor) * NPC_FRAMES_COLS;
        mCurrFrame += static_cast<int>(mState) * NPC_TRADE_FRAMES;
        mCurrFrame += mCurrStep;
    }
}


// Display NPC to user
void NPC::render()
{
    mTexturePtr->render(x, y, &mTextureFrames[static_cast<int>(mCurrFrame)]);
}


// Returns the EEntityType of the NPC
EEntityType NPC::getType()
{
    return EEntityType::NPC;
}