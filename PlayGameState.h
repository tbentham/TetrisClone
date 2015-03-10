#ifndef PLAYGAMESTATE_H_INCLUDED
#define PLAYGAMESTATE_H_INCLUDED

#include "GameState.h"

#include <SDL_ttf.h>

#include "Field.h"
#include "Texture.h"

enum PlayState
{
    PLAY_NORMAL
};

class PlayGameState : public GameState
{
private:
    TTF_Font* loadedFont;
    Texture scoreTexture; // TODO: Move to Field class
    int displayedScore;
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
