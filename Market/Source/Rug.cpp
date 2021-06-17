/**
* Title: Market
* Author: Tonia Sanzo
* Date: 6/11/21
*
* This is a rug, it uses a mutex to share data between multiple threads
*/
#include "PCH.h"
#include "Rug.h"


// Number of rugs constructed
int Rug::rugCount = 0;


// Default initialize the rug, the rug will not be properly initialized until it is
// initialized with init
Rug::Rug()
{
    lock_guard<mutex> lock(mStateMtx);

    // Initialize with a random state
    mState = static_cast<ETradeState>(rand() % 3);
    rugIdx = ++rugCount;
}


// Deallocate resources used by the  rug
Rug::~Rug()
{}


// Properly initialize the rug 
bool Rug::init(SDLManager* aSDL)
{
    // Initialization success flag
    bool success = true;
    return success;
}


// Draw the rug to the screen
void Rug::render()
{
    lock_guard<mutex> lock(mStateMtx);
    cout << "Rug " << rugIdx << " current state: " << static_cast<int>(mState) << '\n';
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