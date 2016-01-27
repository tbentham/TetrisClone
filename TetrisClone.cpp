#include "TetrisClone.h"

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

void TetrisClone::Init( SDL_Renderer* newRenderer )
{
    mainRenderer = newRenderer;
    if ( mainRenderer == NULL )
    {
        printf( "No valid renderer given. Exiting...\n" );
        isRunning = false;
        return;
    }

    currentState.reset( new TitleGameState() );

    if ( !currentState->Init( mainRenderer ) )
    {
        printf( "State could not be initiated. Exiting...\n" );
        isRunning = false;
        return;
    }

    fadeAlpha = 0xFF;
    fadeLastTime = SDL_GetTicks();

    isRunning = true;
}

void TetrisClone::Cleanup()
{
    currentState->Cleanup();

    isRunning = false;
}

void TetrisClone::HandleEvents()
{
    while ( SDL_PollEvent( &e ) != 0 )
    {
        if ( e.type == SDL_QUIT )
            isRunning = false;
        else if ( currentState->GetGameStateRequest() == -1 )
            currentState->HandleEvent( e );
    }
}

/* Current task: Implement modes/state functionality */
void TetrisClone::Update()
{
    // Check for game state request
    int requestedState = currentState->GetGameStateRequest();
    if ( requestedState != -1 )
    {
        if ( !FadeOut() && ( SDL_GetTicks() - fadeLastTime > 500 || requestedState == GAME_QUIT ) )
        {
            switch ( requestedState )
            {
            case GAME_TITLE:
                currentState.reset( new TitleGameState() );
                currentState->Init( mainRenderer );
                break;
            case GAME_PLAY:
                currentState.reset( new PlayGameState() );
                currentState->Init( mainRenderer );
                break;
            case GAME_SCORE:
                currentState.reset( new ScoreGameState( currentState->GetGlobalScore() ) );
                currentState->Init( mainRenderer );
                break;
            case GAME_QUIT:
                isRunning = false;
                break;
            default:
                printf( "Requested game state invalid. Exiting...\n" );
                isRunning = false;
                break;
            }
        }
    }
    else
    {
        currentState->Update();

        if ( fadeAlpha > 0x00 )
            FadeIn();
    }
}

void TetrisClone::Render()
{
    currentState->Render( mainRenderer );

    if ( fadeAlpha > 0x00 )
    {
        SDL_SetRenderDrawColor( mainRenderer, 0x00, 0x00, 0x00, fadeAlpha );
        SDL_SetRenderDrawBlendMode( mainRenderer, SDL_BLENDMODE_BLEND );
        SDL_RenderFillRect( mainRenderer, NULL );
        SDL_SetRenderDrawBlendMode( mainRenderer, SDL_BLENDMODE_NONE );
    }

    SDL_RenderPresent( mainRenderer );
}

bool TetrisClone::FadeIn()
{
    if ( fadeAlpha == 0x00 )
        return false; // Already at minimum alpha value

    if ( fadeAlpha == 0xFF )
        fadeLastTime = SDL_GetTicks() - fadeDelay; // Reset checkpoint

    int timePassed = ( fadeAlpha == 0xFF ) ? fadeDelay : SDL_GetTicks() - fadeLastTime;

    if ( timePassed >= fadeDelay )
    {
        fadeAlpha = max( fadeAlpha - timePassed / fadeDelay, 0x00 );
        fadeLastTime += timePassed;
    }

    return true;
}

bool TetrisClone::FadeOut()
{
    if ( fadeAlpha == 0xFF )
        return false; // Already at maximum alpha value

    if ( fadeAlpha == 0x00 )
        fadeLastTime = SDL_GetTicks() - fadeDelay; // Reset checkpoint

    int timePassed = SDL_GetTicks() - fadeLastTime;

    if ( timePassed >= fadeDelay )
    {
        fadeAlpha = min( fadeAlpha + timePassed / fadeDelay, 0xFF );
        fadeLastTime += timePassed;
    }

    return true;
 }
