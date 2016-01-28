#include "TitleGameState.h"

#include <stdio.h>
#include <SDL2/SDL_ttf.h>

bool TitleGameState::Init( SDL_Renderer* renderer )
{
    gameStateRequest = -1; // -1 = no request

    TTF_Font* loadedFont = TTF_OpenFont( "the_first_fontstruction.ttf", 72 );
    if( loadedFont == NULL )
    {
        printf( "Failed to load font. SDL_ttf error: %s\n", TTF_GetError() );
        printf( "Failed to load media for title state.\n" );
        return false;
    }

    if( !titleTexture.LoadFromRenderedText( renderer, "TETRIS CLONE", { 0xFF, 0xFF, 0xFF, 0x00 }, loadedFont ) )
        return false;

    TTF_CloseFont( loadedFont );
    loadedFont = NULL;

    titleMenu.LoadTextures( renderer );

    return true;
}

void TitleGameState::Cleanup()
{

}

void TitleGameState::HandleEvent( SDL_Event& e )
{
    if ( e.type == SDL_KEYDOWN )
    {
        switch ( e.key.keysym.sym )
        {
        case SDLK_UP:
            printf( "Key press up\n" );
            titleMenu.ChangeOption( MOVE_UP );
            break;
        case SDLK_DOWN:
            printf( "Key press down\n" );
            titleMenu.ChangeOption( MOVE_DOWN );
            break;
        case SDLK_RETURN:
            printf( "Key press return\n" );
            switch ( titleMenu.GetCurrentOption() )
            {
            case MENU_PLAY:
                printf( "'Play' selected\n" );
                gameStateRequest = GAME_PLAY;
                break;
            case MENU_SCORE:
                printf( "'Score' selected\n" );
                gameStateRequest = GAME_SCORE;
                break;
            case MENU_QUIT:
                printf( "'Quit' selected\n" );
                gameStateRequest = GAME_QUIT;
                break;
            default:
                printf( "Invalid menu option!\n" );
            }
        default:
            break;
        }
    }
}

void TitleGameState::Update()
{

}

void TitleGameState::Render( SDL_Renderer* renderer )
{
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x99, 0xFF );
    SDL_RenderClear( renderer );

    titleTexture.Render( renderer, 50, 30 );

    titleMenu.Render( renderer, 80, 140 );
}
