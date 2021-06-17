/**
* Title: Market
* Author: Tonia Sanzo
* Date: 6/12/21
*
* Texture is a class used for the management of textures(Textures are our images that are used to render to the screen)
*/
#pragma once
#include "PCH.h"
#include "SDL.h"
#include <SDL_image.h>


// Texture wrapper class
class Texture {

public:
    // Initialize variables
    Texture();

    // Deallocate memory
    ~Texture();

    // Loads image at specified path
    bool loadFromFile(std::string path);

    // Creates image from font string
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

    // Deallocates texture
    void free();

    // Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    // Set blending
    void setBlendMode(SDL_BlendMode blending);

    // Set alpha modulation
    void setAlpha(Uint8 alpha);

    // Renders texture at given point
    void render(int x, int y, SDL_Rect* clip = nullptr, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip = SDL_FLIP_NONE);

    // Initialize UTexture
    void initTexture(SDL_Renderer* rend);

    // update the window dimensions
    void updateWindowScale(double sc);

    // Gets image dimensions
    int getHeight() { return mHeight; }
    int getWidth() { return mWidth; }
    double getScale() { return mScale; }
    double getWindowScale() { return mWindowScale; }

    // Set image scale
    void updateScale(double sc);
private:
    // The actual hardware texture, and the games renderer
    SDL_Texture* mTexture;
    SDL_Renderer* mRenderer;


    // Image dimensions
    int mWidth, mHeight;
    double mWindowScale;
    double mScale;
};

// Global mutex used to prevent multiple threads from attempting to use the renderer at the same time
mutex gTextureMtx;