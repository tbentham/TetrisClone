#ifndef TITLEGAMESTATE_H_INCLUDED
#define TITLEGAMESTATE_H_INCLUDED

#include "GameState.h"

#include "Menu.h"

enum TitleState
{
    TITLE_NORMAL
};

/* Game state where title and main menu are displayed */
class TitleGameState : public GameState
{
private:
    Texture titleTexture;

    Menu titleMenu;
public:
    TitleGameState() {};
    ~TitleGameState() { Cleanup(); };

    bool Init( SDL_Renderer* renderer );
    void Cleanup();
    void HandleEvent( SDL_Event& e );
    void Update();
    void Render( SDL_Renderer* renderer );
};

#endif // TITLEGAMESTATE_H_INCLUDED
