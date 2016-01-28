#include "Menu.h"

#include <stdio.h>

Menu::Menu() : currentOption( MENU_PLAY )
{
    menuFont = TTF_OpenFont( "the_first_fontstruction.ttf", 28 );
    if( menuFont == NULL )
    {
        printf( "Failed to load font. SDL_ttf error: %s\n", TTF_GetError() );
        printf( "Failed to load media for title state.\n" );
    }
}

Menu::~Menu()
{
    TTF_CloseFont( menuFont );
}

void Menu::LoadTextures( SDL_Renderer* renderer )
{
    // Load arrow texture

    arrowTexture.LoadFromFile( renderer, "images/arrow.png", { 0x00, 0x80, 0x80, 0xFF } );

    // Load text textures

    char buffer[12];
    SDL_Color textColor = { 0xFF, 0xFF, 0xFF, 0x00 };

    textTextures = std::vector<Texture>( 3 );

    sprintf( buffer, "Play" );
    if( !textTextures[0].LoadFromRenderedText( renderer, buffer, textColor, menuFont ) )
    {
        textTextures.clear();
        return;
    }

    sprintf( buffer, "View scores" );
    if( !textTextures[1].LoadFromRenderedText( renderer, buffer, textColor, menuFont ) )
    {
        textTextures.clear();
        return;
    }

    sprintf( buffer, "Quit" );
    if( !textTextures[2].LoadFromRenderedText( renderer, buffer, textColor, menuFont ) )
    {
        textTextures.clear();
        return;
    }
}

void Menu::ChangeOption( int moveDirection )
{
    switch ( currentOption )
    {
    case MENU_PLAY:
        if ( moveDirection == MOVE_UP )
            currentOption = MENU_QUIT;
        if ( moveDirection == MOVE_DOWN )
            currentOption = MENU_SCORE;
        break;
    case MENU_SCORE:
        if ( moveDirection == MOVE_UP )
            currentOption = MENU_PLAY;
        else if ( moveDirection == MOVE_DOWN )
            currentOption = MENU_QUIT;
        break;
    case MENU_QUIT:
        if ( moveDirection == MOVE_UP )
            currentOption = MENU_SCORE;
        else if ( moveDirection == MOVE_DOWN )
            currentOption = MENU_PLAY;
        break;
    default:
        break;
    }
}

void Menu::Render( SDL_Renderer* renderer, int x, int y )
{
    // Render arrow

    arrowTexture.Render( renderer, x, y + 5 + currentOption * lineSpacing );

    // Render text

    for ( unsigned int i = 0; i < textTextures.size(); i++ )
    {
        textTextures[i].Render( renderer, x + 39, y + i * lineSpacing );
    }
}
