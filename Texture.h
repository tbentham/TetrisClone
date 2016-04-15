#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/* Wrapper for SDL_Texture class ( based on code by Lazy Foo' Productions ) */
class Texture
{
private:
    int tWidth;
    int tHeight;

    SDL_Texture* actualTexture;

public:
    Texture() : tWidth( 0 ), tHeight( 0 ), actualTexture( NULL ) {};
    ~Texture() { Cleanup(); };

    bool LoadFromFile( SDL_Renderer* renderer, std::string path, SDL_Color keyColor = { 0x00, 0x00, 0x00, 0x00 } );
    bool LoadFromRenderedText( SDL_Renderer* renderer, std::string textureText, SDL_Color textColor, TTF_Font* font );
    void Cleanup();
    void Render( SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = NULL );

    bool IsLoaded() { return actualTexture != NULL; }
    int GetWidth() { return tWidth; };
    int GetHeight() { return tHeight; };
};

#endif // TEXTURE_H_INCLUDED
