/**
* Title: Market
* Author: Tonia Sanzo
* Date: 6/9/21
*
* SDLManager is an interface that abstracts away the management(initialization, destruction, etc.) of the
* SDL subsystem.
*/
#include "PCH.h"
#include "SDLManager.h"


// Static window dimensions
uint32_t SDLManager::mWindowWidth  = 0;
uint32_t SDLManager::mWindowHeight = 0;


// Non-existant SDLManager
SDLManager::SDLManager()
{
    mWindowIcon = nullptr;
    mSDLWindow = nullptr;
    mRenderer = nullptr;
    mMouseFocus = mKeyboardFocus = mFullscreen = mMinimized = false;
    mWindowHeight = mWindowWidth = 0;
}


// Frees the resources used by SDLManager, and closes the SDL subsystem
SDLManager::~SDLManager()
{
    // Deallocate window icon
    if (mWindowIcon)
    {
        SDL_FreeSurface(mWindowIcon);
        mWindowIcon = nullptr;
    }

    // Deallocate window
    if (mSDLWindow)
    {
        SDL_DestroyWindow(mSDLWindow);
        mSDLWindow = nullptr;
    }

    // Deallocate renderer
    if (mRenderer)
    {
        SDL_DestroyRenderer(mRenderer);
        mRenderer = nullptr;
    }

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}


// Initialize SDL subsystem and create a window, with the given name, dimensions, and path to the window icon
//
// Returns false on failure
bool SDLManager::init(string aTitle /*= "Balogna Engine"*/, float aWidth /*= 1280.f*/, float aHeight /*= 760.f*/, string aIconPath /*= ""*/, Uint32 aWindowFlags /*= SDL_WINDOW_MAXIMIZED*/)
{
    // Initialization flag
    bool success = true;

    // Initialize SDL subsystems
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
        success = false;
    }
    else
    {
        // Create window
        mSDLWindow = SDL_CreateWindow(aTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, static_cast<uint16_t>(aWidth), static_cast<uint16_t>(aHeight), aWindowFlags);
        SDL_GetWindowSize(mSDLWindow, &mWindowWidth, &mWindowHeight);

        if (mSDLWindow != nullptr)
        {
            mMouseFocus = true;
            mKeyboardFocus = true;
            mRenderer = createRenderer();
        }
        else
        {
            cout << "Failed to create the SDL window!\n";
            success = false;
        }

        // Confirm mRenderer is initialized, and aIconPath is not the empty string
        if (mRenderer && !aIconPath.empty())
        {
            // Create the surface
            mWindowIcon = IMG_Load(aIconPath.c_str());
            if (!mWindowIcon)
            {
                cout << "Unable to load image \"" << aIconPath.c_str() << "\"! SDL_image Error: " << IMG_GetError() << "\n";
                success = false;
            }
            else
            {
                // Set the window icon
                SDL_SetWindowIcon(mSDLWindow, mWindowIcon);
            }
        }
        else
        {
            // Set renderer color (WHITE)
            SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

            // Initialize PNG loading
            uint16_t imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) & imgFlags))
            {
                cout << "SDL_image could not initialzie! SDL_image Error: " << IMG_GetError() << "\n";
                success = false;
            }
        }
    }

    return success;
}


// Creates a pointer to renderer associated with this window object
SDL_Renderer* SDLManager::createRenderer()
{
    return SDL_CreateRenderer(mSDLWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}


// Handle the window events, i.e. KeyboardFocus, etc.
bool SDLManager::handleEvent(SDL_Event& e)
{
    // Window Event occured
    if (e.type == SDL_WINDOWEVENT) {
        switch (e.window.event) {
        // Repaint on exposure
        case SDL_WINDOWEVENT_EXPOSED:
            SDL_RenderPresent(mRenderer);
            break;

        // Mouse entered window
        case SDL_WINDOWEVENT_ENTER:
            mMouseFocus = true;
            break;

        // Mouse left window
        case SDL_WINDOWEVENT_LEAVE:
            mMouseFocus = false;
            break;

        // Window has keyboard focus
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            mKeyboardFocus = true;
            break;

        // Window lost keyboard focus
        case SDL_WINDOWEVENT_FOCUS_LOST:
            mKeyboardFocus = false;
            break;

        // Window minimized
        case SDL_WINDOWEVENT_MINIMIZED:
            mMinimized = true;
            break;

        // Window restored
        case SDL_WINDOWEVENT_RESTORED:
            mMinimized = false;
            break;
        }
    }
    return false;
}


// Center the window position
void SDLManager::center()
{
    SDL_SetWindowPosition(mSDLWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}