#include "PCH.h"
#include "Texture.h"


// Texture constructor
Texture::Texture() {
    // Initialize
    mTexture     = nullptr;
    mRenderer    = nullptr;
    mTextureMtx  = nullptr;
    mWidth       = 0;
    mHeight      = 0;
    mScale       = 1;
    mWindowScale = 1;

}


// Texture destructor
Texture::~Texture() {
    // Deallocate texture if the texture exists
    free();

    // Free renderer and font if necessary
    if (mRenderer) {
        mRenderer = nullptr;
    }
}


// Load Texture from a file
bool Texture::loadFromFile(std::string path, mutex* mtx) {
    // Get rid of preexisting texture
    free();

    // Success flag
    bool success = true;

    if (!mtx)
    {
        cout << "mutex* argument cannot be the nullptr!\n";
        return false;
    }
    else
    {
        mTextureMtx = mtx;
    }

    // Exit prematuraly if Texture has not been properly initialized
    if (!mRenderer) {
        cout << "Attempted to render a texture without initializing a renderer!\n";
        return false;
    }

    // The final texture
    SDL_Texture* newTexture = NULL;

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (!loadedSurface) {
        cout << "unable to load image " << path.c_str() << "! SDL_image Error: " << IMG_GetError() << "\n";
        success = false;
    }
    else {
        // Color key image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
        if (!newTexture) {
            cout << "Unable to create texture from " << path.c_str() << "! SDL Error: " << SDL_GetError() << "\n";
            success = false;
        }
        else {
            // Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    // Return success
    mTexture = newTexture;
    return success && mTexture != nullptr;
}


// Deallocate the texture
void Texture::free() {
    // Free texture if it exists
    if (mTexture) 
    {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
        mWidth = 0;
        mHeight = 0;
        mScale = 0;
    }

    // Mutex is not explicitly deleted because it's managed in Game.cpp
    if (mTextureMtx)
    {
        mTextureMtx = nullptr;
    }
}


// Define whether this texture can blend
void Texture::setColor(Uint8 r, Uint8 g, Uint8 b) {
    // Modulate texture
    SDL_SetTextureColorMod(mTexture, r, g, b);
}


// Define whether this texture can blend
void Texture::setBlendMode(SDL_BlendMode blending) {
    // Set blending function
    SDL_SetTextureBlendMode(mTexture, blending);
}


// Define the texture's opacity
void Texture::setAlpha(Uint8 alpha) {
    // Modulate texture alpha
    SDL_SetTextureAlphaMod(mTexture, alpha);
}


// Render texture
void Texture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    // Set Rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth * mWindowScale, mHeight * mWindowScale };

    // Set clip rendering dimensions
    if (clip != nullptr) {
        renderQuad.w = clip->w * mScale * mWindowScale;
        renderQuad.h = clip->h * mScale * mWindowScale;
    }

    // Render to screen
    lock_guard<mutex> lock(*mTextureMtx);
    SDL_RenderCopyEx(mRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}


// initialize Texture with a renderer
void Texture::initTexture(SDL_Renderer* rend) {
    mRenderer = rend;
}


// Set image scale
void Texture::updateScale(double sc) {
    mScale = sc;
    mWidth = static_cast<double>(mWidth) * sc;
    mHeight = static_cast<double>(mHeight) * sc;
}


// Update window scale
void Texture::updateWindowScale(double sc)
{
    mWindowScale = sc;
}