#ifndef PLAYGAMESTATE_H_INCLUDED
#define PLAYGAMESTATE_H_INCLUDED

#include "GameState.h"

#include "Field.h"

enum PlayState
{
    PLAY_NORMAL
};

/* Game state where game is actually played */
class PlayGameState : public GameState
{
private:
    Field playField;
public:
    PlayGameState() {};
    ~PlayGameState() { Cleanup(); };

    bool Init( SDL_Renderer* renderer );
    void Cleanup();
    void HandleEvent( SDL_Event& e );
    void Update();
    void Render( SDL_Renderer* renderer );
};

#endif // PLAYGAMESTATE_H_INCLUDED
