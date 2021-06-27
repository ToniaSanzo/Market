/**
* Title: Market
* Author: Tonia Sanzo
* Date: 6/9/21
*
* Game manages the game entities (npcs, rugs, etc.)
*/
#include "PCH.h"
#include "Game.h"
#include <SDL.h>
#include "ThreadSafeRNG.h"


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

    // Load the rugs shared resources
    mRugTexture.initTexture(sdl->getRenderer());
    if (!mRugTexture.loadFromFile("assets/rug.png", &mTextureMutex))
    {
        cout << "Failed to load rug sprite sheet!\n";
        success = false;
    }
    else
    {
        mRugTexture.updateScale(3);

        // Set the rug's frames dimensions
        for (uint16_t col = 0; col < RUG_FRAMES; ++col)
        {
            mRugFrames[col].x = col * RUG_FRAME_WIDTH;
            mRugFrames[col].y = 0;
            mRugFrames[col].w = RUG_FRAME_WIDTH;
            mRugFrames[col].h = RUG_FRAME_HEIGHT;
        }

        // Create 1 unique rugs
        for (uint16_t i = 0; i < 1; ++i)
        {
            rugs.push_back(new Rug());
            rugs[i]->init(&mRugTexture, mRugFrames);
        }
    }

    // Load the NPCs shared resources
    mNPCTexture.initTexture(sdl->getRenderer());
    if (!mNPCTexture.loadFromFile("assets/npc.png", &mTextureMutex))
    {
        cout << "Failed to load rug sprite sheet!\n";
        success = false;
    }
    else
    {
        mNPCTexture.updateScale(3);

        // Set the npc's frames dimensions
        for (uint16_t row = 0; row < NPC_FRAMES_ROWS; ++row)
        {
            for (uint16_t col = 0; col < NPC_FRAMES_COLS; ++col)
            {
                mNPCFrames[(row * NPC_FRAMES_COLS) + col].x = col * NPC_FRAME_WIDTH;
                mNPCFrames[(row * NPC_FRAMES_COLS) + col].y = row * NPC_FRAME_HEIGHT;
                mNPCFrames[(row * NPC_FRAMES_COLS) + col].w = NPC_FRAME_WIDTH;
                mNPCFrames[(row * NPC_FRAMES_COLS) + col].h = NPC_FRAME_HEIGHT;
            }
        }

        // Create 1 unique NPCs
        for (uint16_t i = 0; i < 1; ++i)
        {
            npcs.push_back(new NPC());
            npcs[i]->init(&mNPCTexture, mNPCFrames);
        }
    }

    
    // Seed the thread safe random number with a random number
    srand(static_cast<unsigned>(time(nullptr)));
    Seed(rand() % 333);

    return success;
}


// Handle's user events events
bool Game::handleEvent(SDL_Event& e)
{
    if (e.type == SDL_QUIT)
    {
        return true;
    }

    // If someone pressed a key
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            return true;
            break;

        case SDLK_1:
            cout << "SLOW ALL NPCS!\n";
            for (NPC* npc : npcs)
            {
                npc->setSpeed(0);
            }
            break;
        case SDLK_2:
            cout << "RANDOMIZE THE SPEED OF ALL NPCS!\n";
            for (NPC* npc : npcs)
            {
                npc->setSpeed(static_cast<float>(rand()) / RAND_MAX);
            }
            break;

        case SDLK_3:
            cout << "SPEED UP ALL NPCS!\n";
            for (NPC* npc : npcs)
            {
                npc->setSpeed(1);
            }
            break;

        default:
            cout << "Unhandled Key!";
            break;
        }
    }

    return false;
}


// Update the game world
void Game::update(const float& dt)
{
    for (uint16_t i = 0; i < 1; ++i)
    {
        threads.push_back(thread(&NPC::update, npcs[i], dt, RANDOM(), RANDOM()));
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
    // Render the rugs and npcs
    for (uint16_t i = 0; i < 1; ++i)
    {
        threads.push_back(thread(&Rug::render, rugs[i]));
        threads.push_back(thread(&NPC::render, npcs[i]));
    }

    for (thread& thread : threads)
    {
        thread.join();
    }
    threads.clear();
}


// Deallocate the game world
void Game::close()
{
    // Delete rugs
    mRugTexture.free();
    for (auto rug : rugs)
    {
        if (rug)
        {
            delete rug;
            rug = nullptr;
        }
    }
    rugs.clear();

    // Delete npcs
    mNPCTexture.free();
    for (auto npc : npcs)
    {
        if (npc)
        {
            delete npc;
            npc = nullptr;
        }
    }
    npcs.clear();

    // If sdl is a valid pointer change it to a nullptr, no need to explicitly delete the sdl pointer because
    // it's managed by the SDLManager
    if (sdl)
    {
        sdl = nullptr;
    }
}