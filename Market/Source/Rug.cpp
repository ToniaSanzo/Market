/**
* Title: Market
* Author: Tonia Sanzo
* Date: 6/11/21
*
* This is a rug, it uses a mutex to share data between multiple threads
*/
#include "PCH.h"
#include "Rug.h"
#include "SDLManager.h"
#include "World.h"


// Default initialize the rug, the rug will not be properly initialized until it is
// initialized with init
Rug::Rug()
{
    mState = rand() % 3;

    // Generate a random location for the rug to spawn
    mLocation.x = 0;
    mLocation.y = 0;

    mTimeToTrade = RUG_TRADE_TIME;

    // References used to render the rug
    mTexturePtr    = nullptr;
    mTextureFrames = nullptr;
}


// Deallocate resources used by the rug, do not explicitly delete the references 
// as they are shared between each rug instance and deallocated elsewhere
Rug::~Rug()
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


// Properly initialize the rug 
bool Rug::init(Texture* aTxtrPtr, SDL_Rect aTxtrFrames[], World& mWorld)
{
    // Initialization success flag
    bool success = true;

    if (!aTxtrPtr)
    {
        // cout << "Failed, valid Texture pointer is required in Rug::init(Texture*, SDL_Rect*)\n";
        success = false;
    }
    else
    {
        mTexturePtr = aTxtrPtr;
        
        if (!aTxtrFrames)
        {
            // cout << "Failed, valid Texture pointer is required in Rug::init(Texture*, SDL_Rect*)\n";
            success = false;
        }
        else
        {
            mTextureFrames = aTxtrFrames;
        
            // Generate a random location for the rug to spawn
            mLocation.x = static_cast<float>(rand() % SDLManager::mWindowWidth);
            mLocation.y = static_cast<float>(rand() % SDLManager::mWindowHeight);

            mWorld.placeEntity(this);
        }
    }


    return success;
}


/**
* update the rug
*
* @param dt - time passed since last time update was called.
*/
void Rug::update(const float& dt)
{
    if (mTimeToTrade <= 0)
    {
        mTimeToTrade = 0;
        return;
    }
    mTimeToTrade = mTimeToTrade - dt;
}


/**
* Draw just the Rug's trade object to the screen.
*/ 
void Rug::render()
{
    mTexturePtr->render(static_cast<int16_t>(mLocation.x - ((RUG_FRAME_WIDTH * RUG_SCALE) / 2.f)), static_cast<int16_t>(mLocation.y - ((RUG_FRAME_WIDTH * RUG_SCALE) / 2.f)), &mTextureFrames[mState + RUG_FRAME_COLS]);
}


/**
* Draw the full Rug.
*/
void Rug::renderFull()
{
    mTexturePtr->render(static_cast<int16_t>(mLocation.x - ((RUG_FRAME_WIDTH * RUG_SCALE) / 2.f)), static_cast<int16_t>(mLocation.y - ((RUG_FRAME_WIDTH * RUG_SCALE) / 2.f)), &mTextureFrames[mState]);
}


/**
* Set this Rug's current trade state to the argument trade state.
*
* @param aState - The trade state to set the Rug to.
*/
void Rug::setTradeState(const ETradeState& aState)
{
    mTimeToTrade = RUG_TRADE_TIME;

    if (mState != static_cast<uint16_t>(aState))
    {
        mState = static_cast<uint16_t>(aState);
    }
}


/**
* Get this Rug's current trade state.
*
* @return ETradeState The trade state of the Rug.
*/
ETradeState Rug::getTradeState()
{
    switch (mState)
    {
    case 0:
        return ETradeState::CHICKEN;
        break;
    case 1:
        return ETradeState::LAMB;
        break;
    case 2: 
        return ETradeState::BREAD_WINE;
        break;
    default:
        return ETradeState::BREAD_WINE;
    }
}


// Returns the EEntityType of the rug
EEntityType Rug::getType()
{
    return EEntityType::RUG;
}


// Get's this Rug as an Entity pointer
Entity* Rug::getEntity()
{
    return static_cast<Entity*>(this);
}


/**
* Get the Rug's current location.
*
* @return Vector the current location of the Entity.
*/
Vector Rug::getLocation()
{
    return mLocation;
}


/**
* Whether the current can make a trade or not
*
* @return bool True if the rug can trade, otherwise false
*/
bool Rug::canTrade()
{
    if (mTimeToTrade <= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
