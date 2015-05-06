#include "Field.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define min( a, b ) ( (a) > (b) ? (b) : (a) )
#define max( a, b ) ( (a) < (b) ? (b) : (a) )

Field::Field()
{
    squares = new int[240];
    rowCompletion = new bool[24];

    scoreFont = TTF_OpenFont( "the_first_fontstruction.ttf", 28 );
    if( scoreFont == NULL )
    {
        printf( "Failed to load score font. SDL_ttf error: %s\n", TTF_GetError() );
        printf( "Failed to load media for play state.\n" );
    }

    SetUpNewGame();
}

Field::~Field()
{
    TTF_CloseFont( scoreFont );
}

void Field::SetUpNewGame()
{
    int i;

    // Reset game score
    actualScore = 0;
    displayedScore = -1;

    // Set all squares to be empty
    for ( i = 0; i < 240; i++)
        squares[i] = BLOCK_NONE;

    // Label all rows as incomplete
    for ( i = 0; i < 24; i++ )
        rowCompletion[i] = false;

    // Clear active block
    activeBlockType = BLOCK_NONE;
    activeBlockIndices = NULL;

    // Prepare random sequence of block types
    srand( time( NULL ) );
    nextBlockType = rand() % ( BLOCKTYPES_SIZE - 1 );

    // Store current time
    lastTime = SDL_GetTicks();

    // Set initial state
    currentState = FIELD_START;
}

void Field::Update()
{
    switch ( currentState )
    {
    case FIELD_START:
        currentState = FIELD_DESCEND;
        ReleaseNextBlock();
        break;
    case FIELD_DESCEND:
        if ( SDL_GetTicks() - lastTime > max( 1000 / ( actualScore / 500 + 1 ), 100 ) )
        {
            if ( !MoveBlock( TRANSLATE_DOWN ) )
                PlaceBlock();
        }
        break;
    case FIELD_ROWCOMPLETE:
        DestroyRows();
        if ( CheckUpperBoundBroken() )
            currentState = FIELD_GAMEOVER;
        else
        {
            currentState = FIELD_DESCEND;
            ReleaseNextBlock();
        }
        break;
    case FIELD_GAMEOVER:
        break;
    }
}

void Field::Render( SDL_Renderer* renderer, int x, int y )
{
    SDL_Color bColor;

    int i, j;
    for ( i = 0; i < 10; i++ )
    {
        for ( j = 4; j < 24; j++ )
        {
            bColor = GetBlockColor( squares[ j * 10 + i ] );

            if ( bColor.a == 0 )
                continue;

            SDL_SetRenderDrawColor( renderer, bColor.r, bColor.g, bColor.b, bColor.a );

            RenderSquare( renderer, x + i * squareSize, y + ( j - 4 ) * squareSize );
        }
    }

    bColor = GetBlockColor( nextBlockType );

    if ( bColor.a != 0 )
    {
        SDL_SetRenderDrawColor( renderer, bColor.r, bColor.g, bColor.b, bColor.a );
        RenderBlock( renderer, nextBlockType, x + 11 * squareSize, y + 3 * squareSize );
    }

    if ( actualScore != displayedScore )
    {
        char buffer[10];
        SDL_Color textColor = { 0xFF, 0xFF, 0xFF, 0x00 };

        sprintf( buffer, "%d", actualScore );
        if( !scoreTexture.LoadFromRenderedText( renderer, buffer, textColor, scoreFont ) )
        {
            printf( "Failed to render score texture.\n" );
        }

        displayedScore = actualScore;
    }

    scoreTexture.Render( renderer, x + 11 * squareSize, y );
}

SDL_Color Field::GetBlockColor( int blockType )
{
    SDL_Color bColor;

    bColor.a = 0xFF;

    switch ( blockType )
    {
    case BLOCK_I: // red
        bColor.r = 0xFF;
        bColor.g = 0x00;
        bColor.b = 0x00;
        break;
    case BLOCK_J: // blue
        bColor.r = 0x00;
        bColor.g = 0x00;
        bColor.b = 0xFF;
        break;
    case BLOCK_L: // orange
        bColor.r = 0xFF;
        bColor.g = 0xA5;
        bColor.b = 0x00;
        break;
    case BLOCK_O: // yellow
        bColor.r = 0xFF;
        bColor.g = 0xFF;
        bColor.b = 0x00;
        break;
    case BLOCK_S: // green
        bColor.r = 0x00;
        bColor.g = 0xFF;
        bColor.b = 0x00;
        break;
    case BLOCK_T: // magenta
        bColor.r = 0xFF;
        bColor.g = 0x00;
        bColor.b = 0xFF;
        break;
    case BLOCK_Z: // cyan
        bColor.r = 0x00;
        bColor.g = 0xFF;
        bColor.b = 0xFF;
        break;
    default:
        bColor.a = 0x00;
    }

    return bColor;
}

void Field::RenderSquare( SDL_Renderer* renderer, int x, int y )
{
    SDL_Rect square = { x, y, squareSize, squareSize };

    SDL_RenderFillRect( renderer, &square );

    SDL_Color dColor;
    SDL_GetRenderDrawColor( renderer, &(dColor.r), &(dColor.g), &(dColor.b), &(dColor.a) );

    SDL_SetRenderDrawColor( renderer, dColor.r / 2, dColor.g / 2, dColor.b / 2, 0xFF );
    SDL_RenderDrawRect( renderer, &square );
    SDL_SetRenderDrawColor( renderer, dColor.r, dColor.g, dColor.b, 0xFF );
}

void Field::RenderBlock( SDL_Renderer* renderer, int blockType, int x, int y )
{
    switch ( blockType )
    {
    case BLOCK_I:
        RenderSquare( renderer, x + squareSize, y );
        RenderSquare( renderer, x + squareSize, y + squareSize );
        RenderSquare( renderer, x + squareSize, y + squareSize * 2 );
        RenderSquare( renderer, x + squareSize, y + squareSize * 3 );
        break;
    case BLOCK_J:
        RenderSquare( renderer, x + squareSize, y + squareSize );
        RenderSquare( renderer, x + squareSize, y + squareSize * 2 );
        RenderSquare( renderer, x, y + squareSize * 3 );
        RenderSquare( renderer, x + squareSize, y + squareSize * 3 );
        break;
    case BLOCK_L:
        RenderSquare( renderer, x, y + squareSize );
        RenderSquare( renderer, x, y + squareSize * 2 );
        RenderSquare( renderer, x, y + squareSize * 3 );
        RenderSquare( renderer, x + squareSize, y + squareSize * 3 );
        break;
    case BLOCK_O:
        RenderSquare( renderer, x, y + squareSize );
        RenderSquare( renderer, x + squareSize, y + squareSize );
        RenderSquare( renderer, x, y + squareSize * 2 );
        RenderSquare( renderer, x + squareSize, y + squareSize * 2 );
        break;
    case BLOCK_S:
        RenderSquare( renderer, x, y + squareSize );
        RenderSquare( renderer, x, y + squareSize * 2 );
        RenderSquare( renderer, x + squareSize, y + squareSize * 2 );
        RenderSquare( renderer, x + squareSize, y + squareSize * 3 );
        break;
    case BLOCK_T:
        RenderSquare( renderer, x, y + squareSize );
        RenderSquare( renderer, x, y + squareSize * 2 );
        RenderSquare( renderer, x + squareSize, y + squareSize * 2 );
        RenderSquare( renderer, x, y + squareSize * 3 );
        break;
    case BLOCK_Z:
        RenderSquare( renderer, x + squareSize, y + squareSize );
        RenderSquare( renderer, x, y + squareSize * 2 );
        RenderSquare( renderer, x + squareSize, y + squareSize * 2 );
        RenderSquare( renderer, x, y + squareSize * 3 );
        break;
    default:
        break;
    }
}

void Field::ReleaseNextBlock()
{
    activeBlockType = nextBlockType;
    activeBlockIndices = new int[4];

    nextBlockType = rand() % ( BLOCKTYPES_SIZE - 1 );

    switch ( activeBlockType )
    {
    case BLOCK_I:
        activeBlockIndices[0] = 5;
        activeBlockIndices[1] = 15;
        activeBlockIndices[2] = 25;
        activeBlockIndices[3] = 35;
        break;
    case BLOCK_J:
        activeBlockIndices[0] = 15;
        activeBlockIndices[1] = 25;
        activeBlockIndices[2] = 34;
        activeBlockIndices[3] = 35;
        break;
    case BLOCK_L:
        activeBlockIndices[0] = 14;
        activeBlockIndices[1] = 24;
        activeBlockIndices[2] = 34;
        activeBlockIndices[3] = 35;
        break;
    case BLOCK_O:
        activeBlockIndices[0] = 24;
        activeBlockIndices[1] = 25;
        activeBlockIndices[2] = 34;
        activeBlockIndices[3] = 35;
        break;
    case BLOCK_S:
        activeBlockIndices[0] = 14;
        activeBlockIndices[1] = 24;
        activeBlockIndices[2] = 25;
        activeBlockIndices[3] = 35;
        break;
    case BLOCK_T:
        activeBlockIndices[0] = 14;
        activeBlockIndices[1] = 24;
        activeBlockIndices[2] = 25;
        activeBlockIndices[3] = 34;
        break;
    case BLOCK_Z:
        activeBlockIndices[0] = 15;
        activeBlockIndices[1] = 24;
        activeBlockIndices[2] = 25;
        activeBlockIndices[3] = 34;
        break;
    }

    for ( int i = 0; i < 4; i++ )
        squares[ activeBlockIndices[i] ] = activeBlockType;

    lastTime = SDL_GetTicks() - 1000;
}

bool Field::MoveBlock( int moveType )
{
    printf( "Move block...\n" );

    if ( activeBlockIndices == NULL )
        // no active block
        return false;

    int* newBlockIndices = new int[4];

    int i;

    // get new position
    switch ( moveType )
    {
    case TRANSLATE_DOWN:
        printf( "Translate down\n" );
        for ( i = 0; i < 4; i++ )
            newBlockIndices[i] = activeBlockIndices[i] + 10;
        lastTime = SDL_GetTicks();
        break;
    case TRANSLATE_LEFT:
        printf( "Translate left\n" );
        for ( i = 0; i < 4; i++ )
            newBlockIndices[i] = activeBlockIndices[i] - 1;
        break;
    case TRANSLATE_RIGHT:
        printf( "Translate right\n" );
        for ( i = 0; i < 4; i++ )
            newBlockIndices[i] = activeBlockIndices[i] + 1;
        break;
    case ROTATE_LEFT:
        printf( "Rotate left\n" );
        newBlockIndices = GetLeftRotation( activeBlockIndices );
        break;
    default:
        printf( "Invalid move type for translation.\n");
        return false;
    }

    // validate movement
    if ( CheckOverlap( newBlockIndices ) )
        return false;

    // perform movement
    for ( i = 0; i < 4; i++ )
        squares[ activeBlockIndices[i] ] = BLOCK_NONE;
    for ( i = 0; i < 4; i++ )
        squares[ newBlockIndices[i] ] = activeBlockType;

    activeBlockIndices = newBlockIndices;

    return true;
}

int* Field::GetLeftRotation( int* blockIndices )
{
    int* newBlockIndices = new int[4];

    switch ( activeBlockType )
    {
    case BLOCK_I:
        newBlockIndices[0] = blockIndices[2] - 20 / ( blockIndices[2] - blockIndices[1] );
        newBlockIndices[1] = blockIndices[2] - 10 / ( blockIndices[2] - blockIndices[1] );
        newBlockIndices[2] = blockIndices[2];
        newBlockIndices[3] = blockIndices[2] + 10 / ( blockIndices[2] - blockIndices[1] );
        break;
    case BLOCK_J:
        newBlockIndices[0] = blockIndices[1] - ( blockIndices[3] - blockIndices[2] );
        newBlockIndices[1] = blockIndices[1];
        newBlockIndices[2] = blockIndices[3] + ( blockIndices[3] - blockIndices[2] );
        newBlockIndices[3] = blockIndices[1] + ( blockIndices[3] - blockIndices[2] );
        break;
    case BLOCK_L:
        newBlockIndices[0] = blockIndices[1] - ( blockIndices[3] - blockIndices[2] );
        newBlockIndices[1] = blockIndices[1];
        newBlockIndices[2] = blockIndices[1] + ( blockIndices[3] - blockIndices[2] );
        newBlockIndices[3] = blockIndices[0] + ( blockIndices[3] - blockIndices[2] );
        break;
    case BLOCK_O:
        newBlockIndices[0] = blockIndices[2];
        newBlockIndices[1] = blockIndices[0];
        newBlockIndices[2] = blockIndices[3];
        newBlockIndices[3] = blockIndices[1];
        break;
    case BLOCK_S:
        newBlockIndices[0] = blockIndices[3] - ( blockIndices[2] - blockIndices[1] );
        newBlockIndices[1] = blockIndices[3];
        newBlockIndices[2] = blockIndices[2];
        newBlockIndices[3] = blockIndices[2] + ( blockIndices[2] - blockIndices[1] );
        break;
    case BLOCK_T:
        newBlockIndices[0] = blockIndices[1] - ( blockIndices[2] - blockIndices[1] );
        newBlockIndices[1] = blockIndices[1];
        newBlockIndices[2] = blockIndices[0];
        newBlockIndices[3] = blockIndices[2];
        break;
    case BLOCK_Z:
        newBlockIndices[0] = blockIndices[0] - 2 * ( blockIndices[2] - blockIndices[1] );
        newBlockIndices[1] = blockIndices[1];
        newBlockIndices[2] = blockIndices[0] - ( blockIndices[2] - blockIndices[1] );
        newBlockIndices[3] = blockIndices[2];
        break;
    }

    return newBlockIndices;
}

bool Field::CheckOverlap( int* blockIndices )
{
    if ( blockIndices == NULL )
        // no active block
        return false;

    int i, j;

    for ( i = 0; i < 4; i++ )
    {
        if ( blockIndices[i] < 0 || blockIndices[i] >= 240 )
            // overlapping top/bottom borders
            return true;

        if ( abs( activeBlockIndices[i] % 10 - blockIndices[i] % 10 ) > 4 )
            // overlapping left/right borders
            return true;

        if ( squares[ blockIndices[i] ] != BLOCK_NONE )
        {
            bool isOccupied = true;
            for ( j = 0; j < 4; j++ )
            {
                if ( blockIndices[i] == activeBlockIndices[j] )
                {
                    isOccupied = false;
                    break;
                }
            }
            if ( isOccupied )
                // overlapping existing block
                return true;
        }
    }

    return false;
}

void Field::PlaceBlock()
{
    printf( "Place block...\n" );

    activeBlockType = BLOCK_NONE;
    activeBlockIndices = NULL;

    if ( CheckRowComplete() )
        currentState = FIELD_ROWCOMPLETE;
    else if ( CheckUpperBoundBroken() )
        currentState = FIELD_GAMEOVER;
    else
        ReleaseNextBlock();
}

bool Field::CheckRowComplete()
{
    bool rowComplete = false;

    int i, j;

    for ( i = 0; i < 24; i++ )
    {
        rowCompletion[i] = true;
        for ( j = 0; j < 10; j++ )
        {
            if ( squares[ i * 10 + j ] == BLOCK_NONE )
            {
                rowCompletion[i] = false;
                break;
            }
        }

        if ( rowCompletion[i] )
        {
            rowComplete = true;
            actualScore += 100;
        }
    }

    return rowComplete;
}

void Field::DestroyRows()
{
    int i, j;

    for ( i = 0; i < 24; i++ )
    {
        if ( rowCompletion[i] )
        {
            for ( j = 0; j < 10; j++ )
            {
                squares[ i * 10 + j ] = BLOCK_NONE;
            }
        }
    }

    int destroyCount = 0;

    for ( i = 23; i >= 0; i-- )
    {
        if ( rowCompletion[i] )
        {
            destroyCount++;
            rowCompletion[i] = false;
        }
        else
            for ( j = 0; j < 10; j++ )
                squares[ ( i + destroyCount ) * 10 + j ] = squares[ i * 10 + j ];
    }
}

bool Field::CheckUpperBoundBroken()
{
    for ( int i = 0; i < 40; i++ )
    {
        if ( squares[i] != BLOCK_NONE )
        {
            printf( "Square %d occupied\n", i );
            return true;
        }
    }

    return false;
}
