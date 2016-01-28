#ifndef TETRISCLONE_H_INCLUDED
#define TETRISCLONE_H_INCLUDED

#include <memory>

#include "GameState.h"
#include "PlayGameState.h"
#include "TitleGameState.h"
#include "ScoreGameState.h"

class TetrisClone
{
private:
    static const int fadeDelay = 2;

    bool isRunning;

    std::unique_ptr<GameState> currentState; // TODO: Implement game state stack

    SDL_Event e;

    SDL_Renderer* mainRenderer;

    int fadeAlpha;
    int fadeLastTime;

    bool FadeIn();
    bool FadeOut();
public:
    TetrisClone() { isRunning = false; };
    ~TetrisClone() { Cleanup(); };

    void Init( SDL_Renderer* newRenderer );
    void Cleanup();
    void HandleEvents();
    void Update();
    void Render();

    bool IsRunning() { return isRunning; };

    int max( int n1, int n2 ) { return n1 > n2 ? n1 : n2; }
    int min( int n1, int n2 ) { return n1 < n2 ? n1 : n2; }
};

#endif // TETRISCLONE_H_INCLUDED
