#pragma once
#include "PCH.h"
#include "NPC.h"
#include "Game.h"


// Construct a non-initialized NPC
NPC::NPC()
{
    // Set the NPC's member variables to random values
    lock_guard<mutex> lock(mStateMtx);
    mState = static_cast<ETradeState>(rand() % 3);
    mNPCColor = static_cast<EColor>(rand() % 3);
    mCurrStep = rand() % 2;
    mCurrFrame = static_cast<uint16_t>(mNPCColor) * NPC_FRAME_COLS;
    mCurrFrame += static_cast<uint16_t>(mState) * NPC_TRADE_FRAMES;
    mCurrFrame += mCurrStep;

    // Generate a random location to walk too
    mCurrLocation.x = static_cast<float>(rand() % SDLManager::mWindowWidth);
    mCurrLocation.y = static_cast<float>(rand() % SDLManager::mWindowHeight);
    setNewWalkLocation(static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX);
    mTimeSinceDirectionReset = 0;


    // Set random movement speed of the NPC
    float speedRange = MAX_SPEED - MIN_SPEED;
    float currSpeedInRange = mSpeed - MIN_SPEED;
    float animSpeedRange = MAX_ANIMATION_SPEED - MIN_ANIMATION_SPEED;

    setSpeed(static_cast<float>(rand()) / RAND_MAX);
    mCurrAnimTime = 0;

    mTexturePtr = nullptr;
    mTextureFrames = nullptr;

    mWorld = nullptr;
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


bool NPC::init(Texture* aTxtrPtr, SDL_Rect aTxtrFrames[], World* aWorld)
{
    // Initialization success flag
    bool success = true;

    mWorld = aWorld;
    if (mWorld == nullptr)
    {
        cout << "Cannot initilaize NPC without a reference to a World object.\n";
        success = false;
    }
    else
    {
        if (!aTxtrPtr)
        {
            cout << "Failed, valid Texture pointer is required in Rug::init(Texture*, SDL_Rect*)\n";
            success = false;
        }
        else
        {
            mTexturePtr = aTxtrPtr;

            if (!aTxtrFrames)
            {
                cout << "Failed, valid Texture pointer is required in Rug::init(Texture*, SDL_Rect*)\n";
                success = false;
            }
            else
            {
                mTextureFrames = aTxtrFrames;

                // Generate a random location for the NPC to spawn
                mCurrLocation.x = static_cast<float>(rand() % (SDLManager::mWindowWidth));
                mCurrLocation.y = static_cast<float>(rand() % (SDLManager::mWindowHeight));
                mWorld->placeEntity(getEntity());
            }
        }
    }

    return success;
}


/**
* Set this Rug's current trade state to the argument trade state.
*
* @param aState - The trade state to set the Rug to.
*/
void NPC::setTradeState(const ETradeState& aState)
{
    lock_guard<mutex> lock(mStateMtx);
    mState = aState;
}


/**
* Get this NPC's current trade state.
*
* @return ETradeState The trade state of the NPC.
*/
ETradeState NPC::getTradeState()
{
    lock_guard<mutex> lock(mStateMtx);
    return mState;
}


// NPC's have different speeds
void NPC::update(const float& dt, const float& aRandomX, const float& aRandomY)
{
    // Whether the trade state changed and we need to update the current frame
    bool bUpdateFrame = false;
    
    mTimeSinceDirectionReset += dt;
    // Generate a new target location to walk to based on if the target location was reached
    if (bNewWalkLocation)
    {
        setNewWalkLocation(aRandomX, aRandomY);
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
            MATH::clamp(mCurrLocation, Vector{ static_cast<float>(SDLManager::mWindowWidth), static_cast<float>(SDLManager::mWindowHeight), 0 });
            
            mWorld->placeEntity(getEntity());
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
        lock_guard<mutex> lock(mStateMtx);
        mCurrFrame = static_cast<uint16_t>(mNPCColor) * NPC_FRAME_COLS;
        mCurrFrame += static_cast<uint16_t>(mState) * NPC_TRADE_FRAMES;
        mCurrFrame += mCurrStep;
        bUpdateFrame = false;
    }

    if (mTimeSinceDirectionReset > NPC_RESET_DIRECTION_TIME)
    {
        setDirection();
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


// Returns the EEntityType of the NPC
Entity* NPC::getEntity()
{
    return this;
}


/**
* Given two values between [0,1) generate a random location within the window for the NPC to walk to
* 
* @param aRandomX - Value between [0,1) to set the target x coordinate
* @param aRandomY - Value between [0,1) to set the target x coordinate
*/
void NPC::setNewWalkLocation(const float& aRandomX, const float& aRandomY)
{

    // Set target location variables
    mTargetLocation.x = aRandomX * (SDLManager::mWindowWidth);
    mTargetLocation.y = aRandomY * (SDLManager::mWindowHeight);

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
    mTimeSinceDirectionReset = 0;
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
}


/**
* Get the NPC's current location.
*
* @return Vector the current location of the Entity.
*/
Vector NPC::getLocation()
{
    return mCurrLocation;
}