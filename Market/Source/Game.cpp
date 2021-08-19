/**
* Title: Market
* Author: Tonia Sanzo
* Date: 6/9/21
*
* Game manages the game entities (npcs, rugs, etc.)
*/
#include "PCH.h"
#include "Game.h"
#include "ThreadSafeRNG.h"


// Constructor
Game::Game()
{
    sdl = nullptr;
    mLoading = true;
    mInitSuccess = true;
    mCurrLoadingFrame = 0;
}


// Loads the game objects and renders the loading screen while they are initializing
bool Game::start(SDLManager* aSDL)
{
    srand(static_cast<unsigned>(time(0)));

    // Success status of this function
    bool success = true;

    // Save the SDLManager
    sdl = aSDL;

    if (!sdl)
    {
        // cout << "Game::start(SDLManager* aSDL) was passed a nullptr argument.\n";
        success = false;
    }
    else
    {
        // Determine the ratio to scale the background assets by
        float wRatio = static_cast<float>(SDLManager::mWindowWidth) / static_cast<float>(BACKGROUND_WIDTH);
        float hRatio = static_cast<float>(SDLManager::mWindowHeight) / static_cast<float>(BACKGROUND_HEIGHT);
        float backgroundScale = (wRatio > hRatio) ? (wRatio) : (hRatio);
        
        if (!initLoadingScreen(backgroundScale))
        {
            // cout << "Game::start(SDLManager* aSDL) call to initLoadingScreen(..) failed.\n";
            success = false;
        }
        else
        {
            std::thread initThread(&Game::init, this, backgroundScale);

            // Game running flag
            bool quit = false;

            // Event handler
            SDL_Event e;

            // Timing variables
            const uint16_t FPS = 60;
            const uint16_t frameDelay = 1000 / FPS;
            Uint32 fStart;
            uint16_t fTime;
            Uint32 pTime = SDL_GetTicks();
            Uint32 cTime = pTime;

            while (mLoading && !quit)
            {
                fStart = SDL_GetTicks();

                // Exit if the user quits
                while (SDL_PollEvent(&e) != 0)
                {
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                        mLoading = false;
                    }
                }

                // Determine the amount of time in seconds since the last time update was called
                cTime = SDL_GetTicks();

                pTime = cTime;

                // Draw the game world to the screen
                mRendererMutex.lock();
                SDL_SetRenderDrawColor(sdl->getRenderer(), 0xD3, 0xD3, 0xD3, 0xFF);
                SDL_RenderClear(sdl->getRenderer());

                mLoadingBackgroundTexture.render(0, 0, &mLoadingFrames[mCurrLoadingFrame]);
                
                SDL_RenderPresent(sdl->getRenderer());
                mRendererMutex.unlock();

                // This measures how long this iteration of the loop took
                fTime = SDL_GetTicks() - fStart;

                // This keeps us from displaying more frames than 60
                if (frameDelay > fTime)
                {
                    SDL_Delay(frameDelay - fTime);
                }
            }            
            initThread.join();

            if (quit || !mInitSuccess)
            {
                success = false;
            }
        }
    }

    return success;
}


// Initialize the game world
void Game::init(const float& aBackgroundScale)
{
    if (!mWorld.init())
    {
        // cout << "Failed to initialize the World!\n";
        mInitSuccess = false;
    }
    else
    {
        // Load the rugs shared resources
        mRugTexture.initTexture(sdl->getRenderer());
        if (!mRugTexture.loadFromFile("assets/rug.png"))
        {
            // cout << "Failed to load rug sprite sheet!\n";
            mInitSuccess = false;
        }
        else
        {
            mRugTexture.updateScale(RUG_SCALE);

            // Set the rug's frames dimensions
            for (uint16_t row = 0; row < RUG_FRAME_ROWS; ++row)
            {
                for (uint16_t col = 0; col < RUG_FRAME_COLS; ++col)
                {
                    mRugFrames[(row * RUG_FRAME_COLS) + col].x = col * RUG_FRAME_WIDTH;
                    mRugFrames[(row * RUG_FRAME_COLS) + col].y = row * RUG_FRAME_HEIGHT;
                    mRugFrames[(row * RUG_FRAME_COLS) + col].w = RUG_FRAME_WIDTH;
                    mRugFrames[(row * RUG_FRAME_COLS) + col].h = RUG_FRAME_HEIGHT;
                }
            }

            // Create 1 unique rugs
            for (uint16_t i = 0; i < ENTITY_COUNT; ++i)
            {
                rugs.push_back(new Rug());
                rugs[i]->init(&mRugTexture, mRugFrames, mWorld);
            }

            // Load the NPCs shared resources
            mNPCTexture.initTexture(sdl->getRenderer());
            if (!mNPCTexture.loadFromFile("assets/npc.png"))
            {
                // cout << "Failed to load rug sprite sheet!\n";
                mInitSuccess = false;
            }
            else
            {
                mNPCTexture.updateScale(NPC_SCALE);

                // Set the npc's frames dimensions
                for (uint16_t row = 0; row < NPC_FRAME_ROWS; ++row)
                {
                    for (uint16_t col = 0; col < NPC_FRAME_COLS; ++col)
                    {
                        mNPCFrames[(row * NPC_FRAME_COLS) + col].x = col * NPC_FRAME_WIDTH;
                        mNPCFrames[(row * NPC_FRAME_COLS) + col].y = row * NPC_FRAME_HEIGHT;
                        mNPCFrames[(row * NPC_FRAME_COLS) + col].w = NPC_FRAME_WIDTH;
                        mNPCFrames[(row * NPC_FRAME_COLS) + col].h = NPC_FRAME_HEIGHT;
                    }
                }

                // Create unique NPCs
                for (uint16_t i = 0; i < ENTITY_COUNT; ++i)
                {
                    npcs.push_back(new NPC());
                    npcs[i]->init(&mNPCTexture, mNPCFrames, &mWorld);
                }

                // Load the background and scale it to fit the screen
                mBackgroundTexture.initTexture(sdl->getRenderer());
                if (!mBackgroundTexture.loadFromFile("assets/bckgrnd.png"))
                {
                    // cout << "Failed to load the background texture!\n";
                    mInitSuccess = false;
                }
                else
                {
                    mBackgroundTexture.updateScale(aBackgroundScale);

                    // Seed the thread safe random number with a random number
                    Seed_ThreadSafeRNG(rand() % 333);

                    // Order the world partitions in different threads
                    vector<thread> localThreads;

                    // warm up the game so it starts smoothly
                    {
                        update(1.f);
                        update(1.f);
                        this_thread::sleep_for(std::chrono::milliseconds(350));
                        ++mCurrLoadingFrame; 
                        this_thread::sleep_for(std::chrono::milliseconds(180));
                        ++mCurrLoadingFrame; 
                        this_thread::sleep_for(std::chrono::milliseconds(180));
                        ++mCurrLoadingFrame; 
                        this_thread::sleep_for(std::chrono::milliseconds(180));
                        ++mCurrLoadingFrame; 
                        this_thread::sleep_for(std::chrono::milliseconds(180));
                        ++mCurrLoadingFrame;
                        this_thread::sleep_for(std::chrono::milliseconds(180));
                        mCurrLoadingFrame = 0;
                        this_thread::sleep_for(std::chrono::milliseconds(180));
                        ++mCurrLoadingFrame;
                        this_thread::sleep_for(std::chrono::milliseconds(180));
                        ++mCurrLoadingFrame;
                        this_thread::sleep_for(std::chrono::milliseconds(180));
                        ++mCurrLoadingFrame;
                        this_thread::sleep_for(std::chrono::milliseconds(180));
                        ++mCurrLoadingFrame;
                        this_thread::sleep_for(std::chrono::milliseconds(180));
                        ++mCurrLoadingFrame;
                        this_thread::sleep_for(std::chrono::milliseconds(180));
                        mCurrLoadingFrame = 0;
                        this_thread::sleep_for(std::chrono::milliseconds(180));
                        ++mCurrLoadingFrame;
                        this_thread::sleep_for(std::chrono::milliseconds(180));
                        ++mCurrLoadingFrame;
                        this_thread::sleep_for(std::chrono::milliseconds(180));
                        ++mCurrLoadingFrame;
                        this_thread::sleep_for(std::chrono::milliseconds(180));
                        ++mCurrLoadingFrame;
                        this_thread::sleep_for(std::chrono::milliseconds(180));
                        ++mCurrLoadingFrame;
                        this_thread::sleep_for(std::chrono::milliseconds(180));
                        mCurrLoadingFrame = 0;
                        this_thread::sleep_for(std::chrono::milliseconds(180));
                        ++mCurrLoadingFrame;
                        this_thread::sleep_for(std::chrono::milliseconds(180));
                        ++mCurrLoadingFrame;
                        this_thread::sleep_for(std::chrono::milliseconds(180));
                        ++mCurrLoadingFrame;
                        this_thread::sleep_for(std::chrono::milliseconds(400));
                    }
                }
            }
        }
    }

    mLoading = false;
}


/**
* Loads the loading screen assets
* @param aBackgroundScale - Ratio to scale the background assets
* @return {bool} True if we successfully load the assets, otherwise false.
*/
bool Game::initLoadingScreen(const float& aBackgroundScale)
{
    bool success = true;

    // Load the loading screen background and scale it to fit the screen
    mLoadingBackgroundTexture.initTexture(sdl->getRenderer());
    if (!mLoadingBackgroundTexture.loadFromFile("assets/loading_bckgrnd.png"))
    {
        // cout << "Failed to load the loading screen background texture!\n";
        success = false;
    }
    else
    {
        mLoadingBackgroundTexture.updateScale(aBackgroundScale);

        // Set the blood frame dimensions
        for (uint16_t row = 0; row < LOAD_FRAME_ROWS; ++row)
        {
            for (uint16_t col = 0; col < LOAD_FRAME_COLS; ++col)
            {
                mLoadingFrames[(row * LOAD_FRAME_COLS) + col].x = col * BACKGROUND_WIDTH;
                mLoadingFrames[(row * LOAD_FRAME_COLS) + col].y = row * BACKGROUND_HEIGHT;
                mLoadingFrames[(row * LOAD_FRAME_COLS) + col].w = BACKGROUND_WIDTH;
                mLoadingFrames[(row * LOAD_FRAME_COLS) + col].h = BACKGROUND_HEIGHT;
            }
        }
    }
    return success;
}


// Handle's user events
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
        default:
            // cout << "Unhandled Key!\n";
            break;
        }
    }

    return false;
}


// Update the game world
void Game::update(const float& dt)
{
    for (uint16_t i = 0; i < ENTITY_COUNT; ++i)
    {
        threads.push_back(thread(&NPC::update, npcs[i], dt, Rand_ThreadSafeRNG(), Rand_ThreadSafeRNG()));
        threads.push_back(thread(&Rug::update, rugs[i], dt));
    }

    // Order each world parition in seperate threads
    threads.push_back(thread(&World::orderWorld, &mWorld, EWorldPartition::LEFT));
    threads.push_back(thread(&World::orderWorld, &mWorld, EWorldPartition::CENTER));
    threads.push_back(thread(&World::orderWorld, &mWorld, EWorldPartition::RIGHT));

    for (thread& thread : threads)
    {
            thread.join();
    }

    threads.clear();
}


// Render the game world
void Game::render()
{
    mBackgroundTexture.render(0, 0);

    for (Rug* rug : rugs)
    {
        rug->renderFull();
    }

    // Render each world parition in seperate threads
    threads.push_back(thread(&World::render, &mWorld, EWorldPartition::LEFT));
    threads.push_back(thread(&World::render, &mWorld, EWorldPartition::CENTER));
    threads.push_back(thread(&World::render, &mWorld, EWorldPartition::RIGHT));

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

    // If sdl is a valid pointer change it to a nullptr, no need to explicitly delete the
    //  sdl pointer because it's managed by the SDLManager
    if (sdl)
    {
        sdl = nullptr;
    }
}