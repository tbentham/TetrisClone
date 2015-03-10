#ifndef TETRISCLONE_H_INCLUDED
#define TETRISCLONE_H_INCLUDED

#include <memory>

#include "GameState.h"
#include "PlayGameState.h"
#include "TitleGameState.h"

class TetrisClone
{
private:
    static const int fadeDelay = 1;

    bool isRunning;

    std::unique_ptr<GameState> currentState;

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
};

#endif // TETRISCLONE_H_INCLUDED
