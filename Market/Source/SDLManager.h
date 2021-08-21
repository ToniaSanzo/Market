/**
* Title: Market
* Author: Tonia Sanzo
* Date: 6/7/21
*
* SDLManager is an interface that abstracts away the management(initialization, destruction, etc.) of the 
* SDL subsystem.
*/
#pragma once
#include "PCH.h"
#include <SDL.h>
#include <SDL_image.h>

// This class acts as an interface with the SDL Subsystem
class SDLManager
{
private:
    // Window data
    SDL_Window* mSDLWindow;
    SDL_Renderer* mRenderer;
    SDL_Surface* mWindowIcon;

    // Window state flags
    bool mMouseFocus;
    bool mKeyboardFocus;
    bool mFullscreen;
    bool mMinimized;

public:
    // Construct SDL subsystem
    SDLManager();

    // Cleanup the SDL subsystem
    ~SDLManager();

    // Initialize SDL subsystem and create a window, with the given name, dimensions, and path to the window icon
    //
    // Returns false on failure
    bool init(string = "Balogna Engine", float = 1280.f, float = 760.f, string = "", Uint32 = SDL_WINDOW_SHOWN);

    // Creates renderer from internal window
    SDL_Renderer* createRenderer();

    // Handles window events
    bool handleEvent(SDL_Event&);

    // Window updater methods
    void center();

    // Return a pointer to the renderer
    SDL_Renderer* getRenderer()
    {
        return mRenderer;
    }

    // Check if the mouse is in the window
    bool hasMouseFouce()
    {
        return mMouseFocus;
    }

    // Check if the keyboard can interact with the window
    bool hasKeyboardFocus()
    {
        return mKeyboardFocus;
    }

    // Check if the window is minimized
    bool isMinimized()
    {
        return mMinimized;
    }

    // Contains the windows width and height
    static int mWindowWidth;
    static int mWindowHeight;
};
