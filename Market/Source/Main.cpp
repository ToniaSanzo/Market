/**
* Title: Market
* Author: Tonia Sanzo
* Date: 6/7/21
* 
* Main.cpp contains the main entry point of the program, and manages the SDL
* subsystem. 
* 
* WARNING!!! Uses floats and int16_t so these are small values and can result
*            in unintended integer wrapping not intended to be a full game 
*            engine!
*/
#include "PCH.h"
#include "SDLManager.h"
#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>


int main(int argc, char* args[])
{
    SDLManager sdl;

    // Initialize SDL subsystem
    if (!sdl.init())
    {
        cout << "Failed to initialize SDL!\n";
    }
    else
    {
        // Game
        Game game;
        game.init(&sdl);

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

        // dt(delta time) - time in seconds since the last update function was called
        float dt = 0.0;

        while (!quit)
        {
            fStart = SDL_GetTicks();

            // Handle events
            while (SDL_PollEvent(&e) != 0)
            {
                quit = game.handleEvent(e); 
            }

            // Determine the amount of time in seconds since the last time update was called
            cTime = SDL_GetTicks();
            dt = static_cast<float>(cTime - pTime) / 1000.f;
            pTime = cTime;

            game.update(dt);

            // Draw the game world to the screen
            SDL_SetRenderDrawColor(sdl.getRenderer(), 0xD3, 0xD3, 0xD3, 0xFF);
            SDL_RenderClear(sdl.getRenderer());
 
            game.render();

            SDL_RenderPresent(sdl.getRenderer());

            // This measures how long this iteration of the loop took
            fTime = SDL_GetTicks() - fStart;

            // This keeps us from displaying more frames than 60
            if (frameDelay > fTime)
            {
                SDL_Delay(frameDelay - fTime);
            }
        }

        game.close();
    }

    return 0;
}
 