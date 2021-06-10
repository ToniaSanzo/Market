/**
* Title: Market
* Author: Tonia Sanzo
* Date: 6/9/21
*
* Game manages the game entities (npcs, rugs, etc.)
*/
#include "pch.h"
#include "Game.h"


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
    
    return true;
}


// Update the game world
void Game::update(const float& dt)
{

}


// Render the game world
void Game::render()
{

}


// Deallocate the game world
void Game::close()
{
    // If sdl is a valid pointer change it to a nullptr, no need to explicitly delete the sdl pointer because it's managed by the SDLManager
    if (sdl)
    {
        sdl = nullptr;
    }
}