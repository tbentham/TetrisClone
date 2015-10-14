#include "Texture.h"

#include <stdio.h>
#include <SDL_image.h>

bool Texture::LoadFromFile( SDL_Renderer* renderer, std::string path, SDL_Color keyColor )
{
    Cleanup();

    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if ( loadedSurface == NULL )
    {
        printf("Unable to load image at %s! SDL_image error: %s\n", path.c_str(), IMG_GetError() );
        return false;
    }

    if ( keyColor.r != 0x00 || keyColor.g != 0x00 || keyColor.b != 0x00 || keyColor.a != 0x00 )
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, keyColor.r, keyColor.g, keyColor.b ) );

    SDL_Texture* finalTexture = NULL;
    finalTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
    if ( finalTexture == NULL )
        printf("Unable to create texture from %s. SDL error: %s\n", path.c_str(), SDL_GetError() );
    else
    {
        tWidth = loadedSurface->w;
        tHeight = loadedSurface->h;
    }

    SDL_FreeSurface( loadedSurface );

    actualTexture = finalTexture;

    return actualTexture != NULL;
}

bool Texture::LoadFromRenderedText( SDL_Renderer* renderer, std::string textureText, SDL_Color textColor, TTF_Font* font )
{
	Cleanup();

	SDL_Surface* textSurface = TTF_RenderText_Solid( font, textureText.c_str(), textColor );
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface. SDL_ttf error: %s\n", TTF_GetError() );
        printf( "Failed to render text texture for '%s'.\n", textureText.c_str() );
		return false;
	}

    actualTexture = SDL_CreateTextureFromSurface( renderer, textSurface );
    if( actualTexture == NULL )
    {
        printf( "Unable to create texture from rendered text. SDL error: %s\n", SDL_GetError() );
        printf( "Failed to render text texture for '%s'.\n", textureText.c_str() );
        SDL_FreeSurface( textSurface );
        return false;
    }

    tWidth = textSurface->w;
    tHeight = textSurface->h;

    SDL_FreeSurface( textSurface );

	return true;
}

void Texture::Cleanup()
{
    if ( actualTexture != NULL )
    {
        SDL_DestroyTexture( actualTexture );
        actualTexture = NULL;
        tWidth = 0;
        tHeight = 0;
    }
}

void Texture::Render( SDL_Renderer* renderer, int x, int y )
{
    SDL_Rect renderQuad = { x, y, tWidth, tHeight };
    SDL_RenderCopy( renderer, actualTexture, NULL, &renderQuad );
}
