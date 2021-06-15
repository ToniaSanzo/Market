/**
* Title: Market
* Author: Tonia Sanzo
* Date: 6/9/21
*
* Game manages the game entities (npcs, rugs, etc.)
*/
#include "pch.h"
#include "Game.h"
#include <SDL.h>
#include "ThreadSafeRandom.h"


// Initialize game entities
Game::Game()
{
    sdl = nullptr;
}

 
// Initialize the game world
bool Game::init(SDLManager* aSDL)
{
    // Seed rand
    srand(static_cast<unsigned>(time(0)));

    // Initialize success flag
    bool success = true;

    // Set the renderer and window
    sdl = aSDL;

    // If sdl is not a valid pointer
    if (!sdl)
    {
        cout << "Game::init(SDLManager*) was passed an invalid argument.\n";
        success = false;
    }

    // Create 5 unique rugs
    for (int i = 0; i < 5; ++i)
    {
        rugs.push_back(new Rug());
    }
    
    // Seed the thread safe random
    Seed(333);

    return true;
}

void Game::randomRugUpdate(int i, vector<Rug*>& rugs)
{
    // Generate the target rug and the new state for the rug
    int trgtRug = static_cast<int>(RANDOM() * 5.f);
    ETradeState newState = static_cast<ETradeState>(RANDOM() * 3.f);

    (*rugs[trgtRug]).updateState(newState);
}

// Update the game world
void Game::update(const float& dt)
{
    for (int i = 0; i < 5; ++i)
    {
        threads.push_back(thread(&randomRugUpdate, i, ref(rugs)));
    }

    for (thread& thread : threads)
    {
        thread.join();
    }
    threads.clear();
}


// Render the game world
void Game::render()
{
    for (int i = 0; i < 5; ++i)
    {
        (*rugs[i]).render();
        SDL_Delay(5);
    }
}


// Deallocate the game world
void Game::close()
{
    // Delete the rugs
    for (auto rug : rugs)
    {
        if (rug)
        {
            delete rug;
            rug = nullptr;
        }
    }
    rugs.clear();

    // If sdl is a valid pointer change it to a nullptr, no need to explicitly delete the sdl pointer because it's managed by the SDLManager
    if (sdl)
    {
        sdl = nullptr;
    }
}