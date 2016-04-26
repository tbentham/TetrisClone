#include "TetrisClone.h"

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

/* Perform initial set-up for the game */
void TetrisClone::Init( SDL_Renderer* newRenderer )
{
    // set game renderer
    mainRenderer = newRenderer;
    if ( mainRenderer == NULL ) // no renderer set
    {
        printf( "No valid renderer given. Exiting...\n" );
        isRunning = false; // request game to stop running
        return;
    }

    // set the initial game state
    currentState.reset( new TitleGameState() );

    // initiate game state
    if ( !currentState->Init( mainRenderer ) ) // game state could not be initiated
    {
        printf( "State could not be initiated. Exiting...\n" );
        isRunning = false; // request game to stop running
        return;
    }

    fadeAlpha = 0xFF; // set fade state to be fully faded out
    fadeLastTime = SDL_GetTicks(); // set time of last fade step to current time

    isRunning = true;
}

/* Perform cleanup for the current game state and finish the game */
void TetrisClone::Cleanup()
{
    currentState->Cleanup();

    isRunning = false; // request game to stop running
}

/* Handle events for the current game state */
void TetrisClone::HandleEvents()
{
    while ( SDL_PollEvent( &e ) != 0 ) // there is an event yet to be handled
    {
        if ( e.type == SDL_QUIT ) // the event is a request to quit
            isRunning = false; // request game to stop runnning
        else if ( currentState->GetGameStateRequest() == -1 ) // no game state being requested
            currentState->HandleEvent( e );
    }
}

/* Update the current game state */
void TetrisClone::Update()
{
    // Check for game state request
    int requestedState = currentState->GetGameStateRequest();

    if ( requestedState != -1 ) // request found
    {
        if ( !FadeOut() && ( SDL_GetTicks() - fadeLastTime > 500 || requestedState == GAME_QUIT ) )
        // finished fading out (and pausing, if between states)
        {
            // set new game state (or quit game) according to request
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
    else // no request
    {
        currentState->Update();

        if ( fadeAlpha > 0x00 ) // not fully faded in
            FadeIn();
    }
}

/* Render the game */
void TetrisClone::Render()
{
    // render the current state
    currentState->Render( mainRenderer );

    if ( fadeAlpha > 0x00 ) // not fully faded in
    {
        // render black for fade in/out transition
        SDL_SetRenderDrawColor( mainRenderer, 0x00, 0x00, 0x00, fadeAlpha );
        SDL_SetRenderDrawBlendMode( mainRenderer, SDL_BLENDMODE_BLEND );
        SDL_RenderFillRect( mainRenderer, NULL );
        SDL_SetRenderDrawBlendMode( mainRenderer, SDL_BLENDMODE_NONE );
    }

    // display final image
    SDL_RenderPresent( mainRenderer );
}

/* Step fade value towards fading in to the current state */
bool TetrisClone::FadeIn()
{
    if ( fadeAlpha == 0x00 ) // already fully faded in
        return false;  // confirm - no longer fading in

    if ( fadeAlpha == 0xFF ) // first fade step
        // update time of last fade step
        fadeLastTime = SDL_GetTicks() - fadeDelay;

    // get time passed since last fade step
    int timePassed = SDL_GetTicks() - fadeLastTime;

    if ( timePassed >= fadeDelay ) // sufficient time has passed
    {
        // step fade value according to time passed, and within lower bound
        fadeAlpha = max( fadeAlpha - timePassed / fadeDelay, 0x00 );
        fadeLastTime += timePassed; // record time of fade step
    }

    return true; // confirm - still fading in
}

/* Step fade value towards fading out of the current state */
bool TetrisClone::FadeOut()
{
    if ( fadeAlpha == 0xFF ) // already fully faded out
        return false; // confirm - no longer fading out

    if ( fadeAlpha == 0x00 ) // first fade step
        // update time of last fade step
        fadeLastTime = SDL_GetTicks() - fadeDelay;

    // get time passed since last fade step
    int timePassed = SDL_GetTicks() - fadeLastTime;

    if ( timePassed >= fadeDelay ) // sufficient time has passed
    {
        // step fade value according to time passed, and within upper bound
        fadeAlpha = min( fadeAlpha + timePassed / fadeDelay, 0xFF );
        fadeLastTime += timePassed; // record time of fade step
    }

    return true; // confirm - still fading out
 }
