#include "PCH.h"
#include "Texture.h"


mutex Texture::mRenderingMtx;


// Texture constructor
Texture::Texture() 
{
    mTexture     = nullptr;
    mRenderer    = nullptr;
    mWidth       = 0;
    mHeight      = 0;
    mScale       = 1;
    mWindowScale = 1;

}


// Texture destructor
Texture::~Texture() 
{
    // Deallocate texture if the texture exists
    free();

    // Free renderer and font if necessary
    if (mRenderer) {
        mRenderer = nullptr;
    }
}


// Load Texture from a file
bool Texture::loadFromFile(std::string path) 
{
    // Get rid of preexisting texture
    free();

    // Success flag
    bool success = true;
    lock_guard<mutex> lock(mTextureMtx);


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
        lock_guard<mutex> lock(mRenderingMtx);
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
    lock_guard<mutex> lock(mTextureMtx);

    // Free texture if it exists
    if (mTexture) 
    {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
        mWidth = 0;
        mHeight = 0;
        mScale = 0;
    }
}


// Define whether this texture can blend
void Texture::setColor(Uint8 r, Uint8 g, Uint8 b) 
{
    lock_guard<mutex> lock(mTextureMtx);

    // Modulate texture
    SDL_SetTextureColorMod(mTexture, r, g, b);
}


// Define whether this texture can blend
void Texture::setBlendMode(SDL_BlendMode blending) 
{
    lock_guard<mutex> lock(mTextureMtx);
    // Set blending function
    SDL_SetTextureBlendMode(mTexture, blending);
}


// Define the texture's opacity
void Texture::setAlpha(Uint8 alpha) 
{
    lock_guard<mutex> lock(mTextureMtx);
    // Modulate texture alpha
    SDL_SetTextureAlphaMod(mTexture, alpha);
}


// Render texture
void Texture::render(int16_t x, int16_t y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    // Set Rendering space and render to screen
    SDL_Rect renderQuad = { x, y, static_cast<int>(mWidth * mWindowScale), static_cast<int>(mHeight * mWindowScale) };

    // Set clip rendering dimensions
    if (clip != nullptr) {
        renderQuad.w = static_cast<int>(clip->w * mScale * mWindowScale);
        renderQuad.h = static_cast<int>(clip->h * mScale * mWindowScale);
    }

    // Render to screen
    lock_guard<mutex> lock(mRenderingMtx);
    SDL_RenderCopyEx(mRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}


// initialize Texture with a renderer
bool Texture::initTexture(SDL_Renderer* rend) 
{
    lock_guard<mutex> lock(mRenderingMtx);
    mRenderer = rend;
    return mRenderer;
}


// Set image scale
void Texture::updateScale(double sc) 
{
    lock_guard<mutex> lock(mTextureMtx);
    mScale = sc;
    mWidth = static_cast<uint16_t>(static_cast<double>(mWidth) * sc);
    mHeight = static_cast<uint16_t>(static_cast<double>(mHeight) * sc);
}


// Update window scale
void Texture::updateWindowScale(double sc)
{
    lock_guard<mutex> lock(mTextureMtx);
    mWindowScale = sc;
}