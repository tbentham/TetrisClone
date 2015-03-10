#ifndef TITLEGAMESTATE_H_INCLUDED
#define TITLEGAMESTATE_H_INCLUDED

#include "GameState.h"

#include <SDL_ttf.h>

#include "Texture.h"
#include "Menu.h"

enum TitleState
{
    TITLE_NORMAL
};

class TitleGameState : public GameState
{
private:
    TTF_Font* titleFont;
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
