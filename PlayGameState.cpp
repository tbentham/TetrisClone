#include "PlayGameState.h"

#include <stdio.h>

bool PlayGameState::Init( SDL_Renderer* renderer )
{
    gameStateRequest = -1; // -1 = no request

    playField.SetUpNewGame();

    return true;
}

void PlayGameState::Cleanup()
{

}

void PlayGameState::HandleEvent( SDL_Event& e )
{
    if ( e.type == SDL_KEYDOWN )
    {
        if ( playField.GetCurrentState() == FIELD_DESCEND )
        {
            switch ( e.key.keysym.sym )
            {
            case SDLK_UP:
                printf( "Key press up\n" );
                playField.MoveBlock( ROTATE_LEFT );
                break;
            case SDLK_DOWN:
                printf( "Key press down\n" );
                if ( !playField.MoveBlock( TRANSLATE_DOWN ) )
                    playField.PlaceBlock();
                break;
            case SDLK_LEFT:
                printf( "Key press left\n" );
                playField.MoveBlock( TRANSLATE_LEFT );
                break;
            case SDLK_RIGHT:
                printf( "Key press right\n" );
                playField.MoveBlock( TRANSLATE_RIGHT );
                break;
            case SDLK_ESCAPE:
                printf( "Key press Esc\n" );
                gameStateRequest = GAME_TITLE;
                break;
            default:
                break;
            }
        }
    }
}

void PlayGameState::Update()
{
    playField.Update();

    if ( playField.GetScore() != globalScore )
        globalScore = playField.GetScore();

    if ( playField.GetCurrentState() == FIELD_GAMEOVER )
        gameStateRequest = GAME_SCORE;
}

void PlayGameState::Render( SDL_Renderer* renderer )
{
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x99, 0xFF );
    SDL_RenderClear( renderer );

    const int squareSize = playField.GetSquareSize();
    SDL_Rect rects[2];

    {
        SDL_Rect fieldBorderRect = { 38, 38, 10 * squareSize + 4, 20 * squareSize + 4 };
        SDL_Rect nextBorderRect = { 38 + 11 * squareSize, 38 + 3 * squareSize, 2 * squareSize + 4, 4 * squareSize + 4 };
        rects[0] = fieldBorderRect;
        rects[1] = nextBorderRect;
    }
    SDL_SetRenderDrawColor( renderer, 0x00, 0x80, 0xFF, 0xFF );
    SDL_RenderFillRects( renderer, rects, 2 );

    {
        SDL_Rect fieldRect = { 40, 40, 10 * squareSize, 20 * squareSize };
        SDL_Rect nextRect = { 40 + 11 * squareSize, 40 + 3 * squareSize, 2 * squareSize, 4 * squareSize };
        rects[0] = fieldRect;
        rects[1] = nextRect;
    }
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderFillRects( renderer, rects, 2 );

    playField.Render( renderer, 40, 40 );
}
