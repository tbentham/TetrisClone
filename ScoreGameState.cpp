#include "ScoreGameState.h"

#include <stdio.h>
#include <SDL2/SDL_ttf.h>

bool ScoreGameState::Init( SDL_Renderer* renderer )
{
    gameStateRequest = -1; // -1 = no request

    backgroundTexture.LoadFromFile( renderer, "images/backgroundScore.png" );

    // Initialise titleTexture
    TTF_Font* loadedFont = TTF_OpenFont( "the_first_fontstruction.ttf", 48 );
    if( loadedFont == NULL )
    {
        printf( "Failed to load font. SDL_ttf error: %s\n", TTF_GetError() );
        printf( "Failed to load media for score state.\n" );
        return false;
    }

    if( !headingTexture.LoadFromRenderedText( renderer, "Hi-Scores", { 0xFF, 0xFF, 0xFF, 0x00 }, loadedFont ) )
        return false;

    TTF_CloseFont( loadedFont );
    loadedFont = NULL;

    // Initialise board
    if ( !board.LoadScoreData( "score.bin" ) )
        return false;

    board.SetPlayerScore( newScore );
    if ( board.GetPlayerScore() != -1 )
        board.RankPlayerScore();

    board.LoadTextures( renderer );

    return true;
}

void ScoreGameState::Cleanup()
{

}

void ScoreGameState::HandleEvent( SDL_Event& e )
{
    if ( e.type == SDL_KEYDOWN )
    {
        switch ( e.key.keysym.sym )
        {
        case SDLK_ESCAPE:
            printf( "Key press Esc\n" );
            if ( !board.PlayerBeingRanked() )
                gameStateRequest = GAME_TITLE;
            break;
        case SDLK_RETURN:
            printf( "Key press return\n" );
            if ( board.PlayerBeingRanked() )
                board.CharEnter( '.' );
            else
                gameStateRequest = GAME_TITLE;
            break;
        case SDLK_BACKSPACE:
            printf( "Key press backspace\n" );
            if ( board.PlayerBeingRanked() )
                board.CharBack();
            break;
        default:
            if ( board.PlayerBeingRanked() )
            {
                SDL_Keycode code = e.key.keysym.sym;
                if ( code >= SDLK_0 && code <= SDLK_9 )
                {
                    printf( "Key press number\n" );
                    board.CharEnter( code - SDLK_0 + '0' );
                }
                else if ( code >= SDLK_a && code <= SDLK_z )
                {
                    printf( "Key press letter\n" );
                    board.CharEnter( code - SDLK_a + 'A' );
                }
            }
            break;
        }
    }
}

void ScoreGameState::Update()
{
    board.UpdateCursor();
}

void ScoreGameState::Render( SDL_Renderer* renderer )
{
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x99, 0xFF );
    SDL_RenderClear( renderer );

    backgroundTexture.Render( renderer, 0, 0 );

    headingTexture.Render( renderer, 50, 25 );

    board.Render( renderer, 90, 90 );
}
