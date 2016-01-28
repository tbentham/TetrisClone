#ifndef FIELD_H_INCLUDED
#define FIELD_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Texture.h"

enum BlockType
{
    BLOCK_I = 0,
    BLOCK_J,
    BLOCK_L,
    BLOCK_O,
    BLOCK_S,
    BLOCK_T,
    BLOCK_Z,
    BLOCK_NONE,
    BLOCKTYPES_SIZE
};

enum MoveType
{
    TRANSLATE_DOWN,
    TRANSLATE_LEFT,
    TRANSLATE_RIGHT,
    ROTATE_LEFT,
    ROTATE_RIGHT
};

enum FieldState
{
    FIELD_START,
    FIELD_DESCEND,
    FIELD_ROWCOMPLETE,
    FIELD_GAMEOVER
};

class Field
{
private:
    static const int squareSize = 20;

    int actualScore;
    int displayedScore;

    int* squares;

    int activeBlockType;
    int* activeBlockIndices;

    int nextBlockType;

    bool* rowCompletion;

    int lastTime;

    int currentState;

    TTF_Font* scoreFont;
    Texture scoreTexture;
    Texture squareSheetTexture;
    SDL_Rect* squareClips;

    void RenderBlock( SDL_Renderer* renderer, int blockType, int x, int y );

    void ReleaseNextBlock();

    int* GetLeftRotation( int* blockIndices );
    int* GetRightRotation( int* blockIndices );
    bool CheckOverlap( int* blockIndices );
    bool CheckRowComplete();
    void DestroyRows();
    bool CheckUpperBoundBroken();
public:
    Field();
    ~Field();

    void LoadTextures( SDL_Renderer* renderer );

    void SetUpNewGame();

    void Update();
    void Render( SDL_Renderer* renderer, int x, int y );

    bool MoveBlock( int moveType );
    void PlaceBlock();

    int GetSquareSize() { return squareSize; };
    int GetScore() { return actualScore; };
    int GetCurrentState() { return currentState; };
    void SetPhase( int newPhase ) { currentState = newPhase; };
    void SetTimePassed( int newTime ) { lastTime = SDL_GetTicks() - newTime; };
};

#endif // FIELD_H_INCLUDED
