#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

class Texture
{
private:
    int tWidth;
    int tHeight;

    SDL_Texture* actualTexture;

    TTF_Font* tFont;

public:
    Texture() : tWidth( 0 ), tHeight( 0 ), actualTexture( NULL ), tFont( NULL ) {};
    ~Texture() { Cleanup(); };

    bool LoadFromFile( SDL_Renderer* renderer, std::string path, SDL_Color keyColor = { 0x00, 0x00, 0x00, 0x00 } );
    bool LoadFromRenderedText( SDL_Renderer* renderer, std::string textureText, SDL_Color textColor, TTF_Font* font );
    void Cleanup();
    void Render( SDL_Renderer* renderer, int x, int y );

    bool IsLoaded() { return actualTexture != NULL || tFont != NULL; }
    int GetWidth() { return tWidth; };
    int GetHeight() { return tHeight; };
    void SetFont( TTF_Font* newFont ) { tFont = newFont; };
};

#endif // TEXTURE_H_INCLUDED
