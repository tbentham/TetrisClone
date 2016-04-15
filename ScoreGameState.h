#ifndef SCOREGAMESTATE_H_INCLUDED
#define SCOREGAMESTATE_H_INCLUDED

#include "GameState.h"

#include "Scoreboard.h"

enum ScoreState
{
    SCORE_NORMAL
};

/* Game state where scores are recorded and displayed */
class ScoreGameState : public GameState
{
private:
    int newScore;

    Texture headingTexture;
    Scoreboard board;
public:
    ScoreGameState( int score ) : newScore( score ) {};
    ~ScoreGameState() { Cleanup(); };

    bool Init( SDL_Renderer* renderer );
    void Cleanup();
    void HandleEvent( SDL_Event& e );
    void Update();
    void Render( SDL_Renderer* renderer );
};

#endif // SCOREGAMESTATE_H_INCLUDED
