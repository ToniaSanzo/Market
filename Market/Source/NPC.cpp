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
    mCurrLocation.x = static_cast<float>(rand() % SDLManager::mWindowWidth);
    mCurrLocation.y = static_cast<float>(rand() % SDLManager::mWindowHeight);

    // Generate a random location to walk too
    setNewWalkLocation(static_cast<float>(rand() % SDLManager::mWindowWidth), static_cast<float>(rand() % SDLManager::mWindowHeight));

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
        float changeInX = mTargetLocation.x - mCurrLocation.x;
        float changeInY = mTargetLocation.y - mCurrLocation.y;

        // If NPC is in range of new location set the bNewWalkLocation flag
        if ((changeInX * changeInX) + (changeInY * changeInY) < (NPC_TARGET_LOCATION_RANGE * NPC_TARGET_LOCATION_RANGE))
        {
            bNewWalkLocation = true;
        }
        // Otherwise, move closer to the target location
        else
        {
            mCurrLocation.x += mDirection.x * (dt * mSpeed);
            mCurrLocation.y += mDirection.y * (dt * mSpeed);

            if (mCurrLocation.x < 0 || mCurrLocation.x > SDLManager::mWindowWidth || mCurrLocation.y < 0 || mCurrLocation.y > SDLManager::mWindowHeight)
            {
                setDirection();
            }
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
    mTexturePtr->render
    (
        static_cast<int16_t>(mCurrLocation.x - ((NPC_FRAME_WIDTH * NPC_SCALE) / 2.f)),
        static_cast<int16_t>(mCurrLocation.y - ((NPC_FRAME_HEIGHT * NPC_SCALE) / 2.f)),
        &mTextureFrames[static_cast<uint16_t>(mCurrFrame)],
        0,
        nullptr,
        (mDirection.x < 0)? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL
    );
}


// Returns the EEntityType of the NPC
EEntityType NPC::getType()
{
    return EEntityType::NPC;
}


// Generate new walk location of the NPC  
void NPC::setNewWalkLocation(const float& aRandomX, const float& aRandomY)
{
    if (aRandomX > SDLManager::mWindowWidth || aRandomY > SDLManager::mWindowHeight || aRandomX < 0 || aRandomY < 0)
    {
        cout << "LOCATION SET OUT OF BOUNDS! {x: " << aRandomX << ", y: " << aRandomY << "}\n";
    }

    // Set target location variables
    mTargetLocation.x = aRandomX;
    mTargetLocation.y = aRandomY;

    setDirection();

    // bNewWalkLocation disabled, prevents setting the target location more than once
    bNewWalkLocation = false;
}


// Set NPC's direction
void NPC::setDirection()
{
    // The vector between current and target location
    float deltaX = mTargetLocation.x - mCurrLocation.x;
    float deltaY = mTargetLocation.y - mCurrLocation.y;

    // Normalize the vector
    float hypotenuse = static_cast<float>(sqrt((pow(deltaX, 2) + pow(deltaY, 2))));
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

    cout << "mSpeed: " << mSpeed << ", mAnimationSpeed: " << mAnimationSpeed << "\n";
}