#include "TetrisClone.h"

#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

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
        if ( !FadeOut() && ( SDL_GetTicks() - fadeLastTime > 500 || requestedState == SDL_QUIT ) )
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
            //case GAME_SCORE:
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
    if ( fadeAlpha <= 0x00 )
        return false;

    int currentTime = SDL_GetTicks();
    if ( currentTime - fadeLastTime > fadeDelay )
    {
        fadeAlpha--;
        fadeLastTime = currentTime;
    }

    return true;
}

bool TetrisClone::FadeOut()
{
    if ( fadeAlpha >= 0xFF )
        return false;

    int currentTime = SDL_GetTicks();
    if ( currentTime - fadeLastTime > fadeDelay )
    {
        fadeAlpha++;
        fadeLastTime = currentTime;
    }

    return true;
 }
