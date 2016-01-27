#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

#include "Texture.h"

enum MenuOption
{
    MENU_PLAY = 0,
    MENU_SCORE,
    MENU_QUIT
};

enum MoveDirection
{
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT
};

class Menu
{
private:
    static const int lineSpacing = 40;

    int currentOption;

    Texture arrowTexture;

    TTF_Font* menuFont;
    std::vector<Texture> textTextures;
public:
    Menu();
    ~Menu();

    void LoadTextures( SDL_Renderer* renderer );

    void ChangeOption( int moveDirection );

    void Render( SDL_Renderer* renderer, int x, int y );

    bool TexturesLoaded() { return !textTextures.empty() && arrowTexture.IsLoaded(); };
    int GetCurrentOption() { return currentOption; };
};

#endif // MENU_H_INCLUDED
