#ifndef SCOREGAMESTATE_H_INCLUDED
#define SCOREGAMESTATE_H_INCLUDED

#include "GameState.h"

#include <iostream>
#include <fstream>
#include <SDL_ttf.h>

#include "Texture.h"

enum ScoreState
{
    SCORE_NORMAL
};

class ScoreGameState : public GameState
{
private:
    static const int listSize = 10;
    // TODO: Create List class (and move the following) (?)
    char* nameList;
    int* scoreList;

    int newScore;

    std::fstream scoreFile; // TODO: Implement file read/write with SDL_RWops

    Texture headingTexture;

    TTF_Font* listFont;
    Texture listTextures;

public:
    ScoreGameState() {};
    ~ScoreGameState() { Cleanup(); };

    bool Init( SDL_Renderer* renderer );
    void Cleanup();
    void HandleEvent( SDL_Event& e );
    void Update();
    void Render( SDL_Renderer* renderer );
};

#endif // SCOREGAMESTATE_H_INCLUDED
