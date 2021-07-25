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
    lock_guard<mutex> lock(mStateMtx);

    mState = static_cast<ETradeState>(rand() % 3);

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
        
            // Generate a random location for the rug to spawn
            mLocation.x = rand() % SDLManager::mWindowWidth;
            mLocation.y = rand() % SDLManager::mWindowHeight;

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
    lock_guard<mutex> lock(mTimeMtx);
    if (mTimeToTrade <= 0)
    {
        mTimeToTrade = 0;
        return;
    }
    mTimeToTrade -= dt;
}

// Draw the rug to the screen
void Rug::render()
{
    lock_guard<mutex> lock(mStateMtx);
    mTexturePtr->render(mLocation.x - ((RUG_FRAME_WIDTH * RUG_SCALE) / 2.f), mLocation.y - ((RUG_FRAME_WIDTH * RUG_SCALE) / 2.f), &mTextureFrames[static_cast<uint16_t>(mState) + RUG_FRAME_COLS]);
}


// Draw the full rug entity to the screen
void Rug::renderFull()
{
    lock_guard<mutex> lock(mStateMtx);
    mTexturePtr->render(mLocation.x - ((RUG_FRAME_WIDTH * RUG_SCALE) / 2.f), mLocation.y - ((RUG_FRAME_WIDTH * RUG_SCALE) / 2.f), &mTextureFrames[static_cast<uint16_t>(mState)]);
}


/**
* Set this Rug's current trade state to the argument trade state.
*
* @param aState - The trade state to set the Rug to.
*/
void Rug::setTradeState(const ETradeState& aState)
{
    lock_guard<mutex> lock_time(mTimeMtx);
    mTimeToTrade = RUG_TRADE_TIME;

    lock_guard<mutex> lock_state(mStateMtx);
    if (aState != mState)
    {
        mState = aState;
    }
}


/**
* Get this Rug's current trade state.
*
* @return ETradeState The trade state of the Rug.
*/
ETradeState Rug::getTradeState()
{
    lock_guard<mutex> lock(mStateMtx);
    return mState;
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
    lock_guard<mutex> lock(mTimeMtx);
    if (mTimeToTrade <= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
