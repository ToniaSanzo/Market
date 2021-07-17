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


// Number of rugs constructed
uint16_t Rug::sRugCount = 0;


// Default initialize the rug, the rug will not be properly initialized until it is
// initialized with init
Rug::Rug()
{
    lock_guard<mutex> lock(mStateMtx);
    mRugIdx = ++sRugCount;

    // Initialize with a random state
    mState = static_cast<ETradeState>(rand() % 3);

    // Generate a random location for the rug to spawn
    mLocation.x = 0;
    mLocation.y = 0;

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

            mWorld.addEntity(this, mLocation);
        }
    }


    return success;
}


// Draw the rug to the screen
void Rug::render()
{
    lock_guard<mutex> lock(mStateMtx);
    mTexturePtr->render(mLocation.x - ((RUG_FRAME_WIDTH * RUG_SCALE) / 2.f), mLocation.y - ((RUG_FRAME_WIDTH * RUG_SCALE) / 2.f), &mTextureFrames[static_cast<uint16_t>(mState)]);
}


// Update the state of the rug
void Rug::updateState(const ETradeState& aState)
{
    lock_guard<mutex> lock(mStateMtx);
    if (aState != mState)
    {
        mState = aState;
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
* @return Vector3 the current location of the Entity.
*/
Vector3 Rug::getLocation()
{
    return mLocation;
}
