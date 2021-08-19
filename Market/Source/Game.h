/**
* Title: Market
* Author: Tonia Sanzo
* Date: 6/9/21
*
* Game manages the game entities (npcs, rugs, etc.)
*/
#pragma once
#include "PCH.h"
#include "SDLManager.h"
#include "Rug.h"
#include "NPC.h"
#include "World.h"


class Game
{
private:
    // Pointer to the SDL system
    SDLManager* sdl;
    mutex mSDLLock;
    atomic<bool> mLoading;
    atomic<bool> mInitSuccess;

    // Mutex to prevent the renderer from being used by more than one thread at a time
    mutex mRendererMutex;

    // Collection of rugs, and shared resources used by the rug
    Texture mRugTexture;
    SDL_Rect mRugFrames[RUG_FRAME_COLS * RUG_FRAME_ROWS];
    vector<Rug*> rugs;

    // Collection of NPCs, and shared resources used by the rug
    Texture mNPCTexture;
    SDL_Rect mNPCFrames[NPC_FRAME_COLS * NPC_FRAME_ROWS];
    vector<NPC*> npcs;

    // The background texture
    Texture mBackgroundTexture;

    // The loading screen textures
    Texture mLoadingBackgroundTexture;
    SDL_Rect mLoadingFrames[TOTAL_LOAD_FRAMES];
    atomic<uint8_t> mCurrLoadingFrame;

    // Our collection of threads
    vector<thread> threads;

    // 2D array with every elements current position and 
    World mWorld;

public:
    // Initializes game entities
    Game();

    // Loads the game objects and renders the loading screen while they are initializing
    bool start(SDLManager*);

    // Handle's user events events
    bool handleEvent(SDL_Event&);

    // Updates the game world
    void update(const float&);

    // Draw the game world
    void render();

    // Free the resources
    void close();

private: 
    // Loads the objects used while the game is running
    void init(const float&);

    // Loads only the loading screen assets
    bool initLoadingScreen(const float&);
};