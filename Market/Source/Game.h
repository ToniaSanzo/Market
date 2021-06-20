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

class Game
{
private:
    // Pointer to the SDL system
    SDLManager* sdl;
    // Mutex to prevent the renderer from being used by more than one thread at a time
    mutex mTextureMutex;

    // Collection of rugs, and shared resources used by the rug
    Texture mRugTexture;
    SDL_Rect mRugFrames[RUG_FRAMES];
    vector<Rug*> rugs;

    // Collection of NPCs, and shared resources used by the rug
    Texture mNPCTexture;
    SDL_Rect mNPCFrames[NPC_FRAMES_COLS * NPC_FRAMES_ROWS];
    vector<NPC*> npcs;

    // Our collection of threads
    vector<thread> threads;
public:
    // Initializes game entities
    Game();

    // Initialize the game objects
    bool init(SDLManager*);

    // Updates the game world
    void update(const float&);

    // Draw the game world
    void render();

    // Free the resources
    void close();
    
    // Update the state of a random rug
    static void randomRugUpdate(int i, vector<Rug*>& rugs);
};