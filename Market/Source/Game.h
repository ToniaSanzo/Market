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

    // Mutex to prevent the renderer from being used by more than one thread at a time
    mutex mTextureMutex;

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
    Texture mLoadingGlassHeartTexture;
    Texture mLoadingBloodTexture;
    SDL_Rect mBloodFrames[LOAD_BLOOD_FRAME_COLS * LOAD_BLOOD_FRAME_ROWS];

    // Our collection of threads
    vector<thread> threads;

    // 2D array with every elements current position and 
    World mWorld;

public:
    // Initializes game entities
    Game();

    // Initialize the game objects
    bool init(SDLManager*);

    // Handle's user events events
    bool handleEvent(SDL_Event&);

    // Updates the game world
    void update(const float&);

    // Draw the game world
    void render();

    // Free the resources
    void close();
};