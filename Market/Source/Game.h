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

class Game
{
private:
    // Pointer to the SDL system
    SDLManager* sdl;

    // Collection of rugs
    vector<Rug*> rugs;

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