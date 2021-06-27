#pragma once
#include "PCH.h"
#include "NPC.h"
#include "Game.h"


// Number of NPCs constructed
uint16_t NPC::sNPCCount = 0;


// Construct a non-initialized NPC
NPC::NPC()
{
    mNPCIdx = ++sNPCCount;

    // Set the NPC's member variables to random values
    mState = static_cast<ETradeState>(rand() % 3);
    mNPCColor = static_cast<EColor>(rand() % 3);
    mCurrStep = rand() % 2;
    mCurrFrame = static_cast<uint16_t>(mNPCColor) * NPC_FRAMES_COLS;
    mCurrFrame += static_cast<uint16_t>(mState) * NPC_TRADE_FRAMES;
    mCurrFrame += mCurrStep;

    // Generate a random location for the NPC to spawn
    mCurrX = rand() % SDLManager::mWindowWidth;
    mCurrY = rand() % SDLManager::mWindowHeight;

    // Generate a random location to walk too
    setNewWalkLocation(rand() % SDLManager::mWindowWidth, rand() % SDLManager::mWindowHeight);

    // Set random movement speed of the NPC
    mSpeed = (rand() % static_cast<uint16_t>(MAX_SPEED - MIN_SPEED)) + MIN_SPEED;
    float speedRange = MAX_SPEED - MIN_SPEED;
    float currSpeedInRange = mSpeed - MIN_SPEED;
    float animSpeedRange = MAX_ANIMATION_SPEED - MIN_ANIMATION_SPEED;

    setSpeed(static_cast<float>(rand()) / RAND_MAX);
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
    
    // Generate a new target location to walk to based on if the target location was reached
    if (bNewWalkLocation)
    {
        setNewWalkLocation(aRandomX * SDLManager::mWindowWidth, aRandomY * SDLManager::mWindowHeight);
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
            mCurrX += mDirection.x * (dt * mSpeed);
            mCurrY += mDirection.y * (dt * mSpeed);
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
        mCurrFrame = static_cast<uint16_t>(mNPCColor) * NPC_FRAMES_COLS;
        mCurrFrame += static_cast<uint16_t>(mState) * NPC_TRADE_FRAMES;
        mCurrFrame += mCurrStep;
        bUpdateFrame = false;
    }
}


// Display NPC to user
void NPC::render()
{
    mTexturePtr->render(mCurrX - (NPC_FRAME_WIDTH / 2), mCurrY - (NPC_FRAME_HEIGHT / 2), &mTextureFrames[static_cast<uint16_t>(mCurrFrame)]);
}


// Returns the EEntityType of the NPC
EEntityType NPC::getType()
{
    return EEntityType::NPC;
}


// Generate new walk location of the NPC  
void NPC::setNewWalkLocation(const float& aRandomX, const float& aRandomY)
{
    // Set target location variables
    mTargetX = aRandomX;
    mTargetY = aRandomY;

    setDirection();

    // bNewWalkLocation disabled, prevents setting the target location more than once
    bNewWalkLocation = false;
}


// Set NPC's direction
void NPC::setDirection()
{
    // The vector between current and target location
    float deltaX = mTargetX - mCurrX;
    float deltaY = mTargetY - mCurrY;

    // Normalize the vector
    float hypotenuse = sqrt((pow(deltaX, 2) + pow(deltaY, 2)));
    mDirection.x = deltaX / hypotenuse;
    mDirection.y = deltaY / hypotenuse;
}


// Set NPC's speed, and animation speed giving a value between 0 and 1, clamped between
// the (MIN_SPEED | MIN_ANIMATION_SPEED) and (MAX_SPEED | MAX_ANIMATION_SPEED)
void NPC::setSpeed(const float& ratio)
{
    // The range between the max and min of the speeds
    float speedRange = MAX_SPEED - MIN_SPEED;
    float animSpeedRange = MIN_ANIMATION_SPEED - MAX_ANIMATION_SPEED;

    // using the ratio, set mSpeed and the mAnimationSpeed
    mSpeed = (speedRange * ratio) + MIN_SPEED;
    mAnimationSpeed = (animSpeedRange * (1.f - ratio)) + MAX_ANIMATION_SPEED;

    cout << "mSpeed: " << mSpeed << ", mAnimationSpeed: " << mAnimationSpeed;
}