#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED

#include <SDL2/SDL.h>
#include "Texture.h"

// Request to change to another game state ( -1 = no change )
enum GameStateRequest
{
    GAME_TITLE,
    GAME_PLAY,
    GAME_SCORE,
    GAME_QUIT
};

/* Superclass for multiple game states */
class GameState
{
protected:
    int gameStateRequest = -1;
    int globalScore = -1;

    Texture backgroundTexture;
public:
    virtual bool Init( SDL_Renderer* renderer ) = 0;
    virtual void Cleanup() = 0;
    virtual void HandleEvent( SDL_Event& e ) = 0;
    virtual void Update() = 0;
    virtual void Render( SDL_Renderer* renderer ) = 0;

    int GetGameStateRequest() { return gameStateRequest; };
    int GetGlobalScore() { return globalScore; };
};

#endif // GAMESTATE_H_INCLUDED
