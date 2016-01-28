#include "Scoreboard.h"

#include <stdio.h>

Scoreboard::Scoreboard()
{
    font = TTF_OpenFont( "the_first_fontstruction.ttf", fontSize );
    if( font == NULL )
    {
        printf( "Failed to load font. SDL_ttf error: %s\n", TTF_GetError() );
        printf( "Failed to load media for score state.\n" );
    }

    // Default values
    playerName = std::string( nameLength, ' ' );
    playerName[0] = '_';
    playerScore = -1;
    playerRank = listSize;
    cursorPos = 0;
    showCursor = true;
    cursorLastTime = SDL_GetTicks();
}

bool Scoreboard::LoadScoreData( const char* fileLocation )
{
    nameList.assign( listSize, std::string( nameLength, 'A' ) );
    scoreList.assign( listSize, 0 );

    int i;

    // Open scores file
    SDL_RWops* file = SDL_RWFromFile( fileLocation, "r+b" );

    // If file doesn't exist, create it
    if ( file == NULL )
    {
        printf( "Warning: Unable to open file! SDL Error %s\n", SDL_GetError() );

        file = SDL_RWFromFile( fileLocation, "w+b" );

        if ( file != NULL )
        {
            printf( "New file created!\n" );

            // Initialise file data
            for ( i = 0; i < listSize; i++ )
            {
                // Set values
                scoreList[i] = ( listSize - i ) * 100;
                // Write to file
                SDL_RWwrite( file, &nameList[i][0], sizeof( char ), nameLength );
            }
            SDL_RWwrite( file, &scoreList[0], sizeof( int ), listSize );

            // Close file handler
            SDL_RWclose( file );
        }
        else
        {
            printf( "Error: Unable to create file! SDL Error %s\n", SDL_GetError() );
            return false;
        }
    }
    // File exists
    else
    {
        printf( "Reading file..." );

        // Set values
        for ( i = 0; i < listSize; i++ )
        {
            SDL_RWread( file, &nameList[i][0], sizeof( char ), nameLength );
        }
        SDL_RWread( file, &scoreList[0], sizeof( int ), listSize );

        // Close file handler
        SDL_RWclose( file );

        printf( " Done!\n" );
    }

    return true;
}

bool Scoreboard::SaveScoreData( const char* fileLocation )
{
    if ( nameList.empty() || scoreList.empty() )
    {
        printf( "Error: Unable to save file! No scores available.\n" );
        return false;
    }

    // Create new scores file (overwriting existing file)
    SDL_RWops* file = SDL_RWFromFile( fileLocation, "w+b" );

    if ( file == NULL )
    {
        printf( "Error: Unable to save file! %s\n", SDL_GetError() );
        return false;
    }
    else
    {
        printf( "Writing to file..." );

        // Write to file
        for ( int i = 0; i < listSize; i++ )
            SDL_RWwrite( file, &nameList[i][0], sizeof( char ), nameLength );
        SDL_RWwrite( file, &scoreList[0], sizeof( int ), listSize );

        // Close file handler
        SDL_RWclose( file );

        printf( " Done!\n");
    }

    return true;
}

bool Scoreboard::LoadTextures( SDL_Renderer* renderer )
{
    // Load arrow texture

    arrowTexture.LoadFromFile( renderer, "images/arrow.png", { 0x00, 0x80, 0x80, 0xFF } );

    // Load text textures

    char buffer[33];
    SDL_Color textColor = { 0xFF, 0xFF, 0xFF, 0x00 };
    int i;

    // Load character textures
    charTextures.assign( 28, Texture() );
    sprintf( buffer, "A" );
    for ( i = 0; i < 26; i++ )
    {
        if( !charTextures[i].LoadFromRenderedText( renderer, buffer, textColor, font ) )
        {
            charTextures.clear();
            return false;
        }
        buffer[0]++;
    }
    sprintf( buffer, "_" );
    if( !charTextures[26].LoadFromRenderedText( renderer, buffer, textColor, font ) )
    {
        charTextures.clear();
        return false;
    }
    sprintf( buffer, "?" );
    if( !charTextures[27].LoadFromRenderedText( renderer, buffer, textColor, font ) )
    {
        charTextures.clear();
        return false;
    }

    // Load number textures
    numTextures.assign( std::max( 10, listSize + 1 ), Texture() );
    for ( i = 0; i < numTextures.size(); i++ )
    {
        sprintf( buffer, "%d", i );
        if( !numTextures[i].LoadFromRenderedText( renderer, buffer, textColor, font ) )
        {
            numTextures.clear();
            return false;
        }
    }

    // Load score textures
    scoreTextures.assign( listSize, Texture() );
    for ( i = 0; i < listSize; i++ )
    {
        sprintf( buffer, "%d", scoreList[i] );
        if( !scoreTextures[i].LoadFromRenderedText( renderer, buffer, textColor, font ))
        {
            scoreTextures.clear();
            return false;
        }
    }

    // Load message texture
    if ( playerScore != -1 )
    {
        if ( playerRank == listSize )
        {
            sprintf( buffer, "%d", playerScore );
            if( !unrankedScoreTexture.LoadFromRenderedText( renderer, buffer, textColor, font ))
                return false;
            sprintf( buffer, "Your score:" );
        }
        else
            sprintf( buffer, "New hi-score!");

        if( !messageTexture.LoadFromRenderedText( renderer, buffer, textColor, font ))
            return false;
    }

    return true;
}

void Scoreboard::Render( SDL_Renderer* renderer, int x, int y )
{
    // Render arrow

    if ( playerScore != -1 && playerRank < listSize )
        arrowTexture.Render( renderer, x, y + 5 + playerRank * lineSpacing );

    // Render score table

    int posY = y;
    int posX = x + 39;
    int charShiftX = fontSize / 2;
    for ( unsigned int i = 0; i < listSize; i++ )
    {
        numTextures[i+1].Render( renderer, posX, posY );

        posX += columnSpacing[0];

        for ( unsigned int j = 0; j < nameLength; j++ )
        {
            if ( nameList[i][j] >= 'A' && nameList[i][j] <= 'Z' )
                charTextures[nameList[i][j]-'A'].Render( renderer, posX, posY );
            else if ( nameList[i][j] >= '0' && nameList[i][j] <= '9' )
                numTextures[nameList[i][j]-'0'].Render( renderer, posX, posY );
            else if ( nameList[i][j] == '_' && showCursor )
                charTextures[26].Render( renderer, posX, posY );

            posX += charShiftX;
        }

        if ( i == playerRank && cursorPos == nameLength && showCursor )
            charTextures[27].Render( renderer, posX, posY );

        posX -= nameLength * charShiftX;

        scoreTextures[i].Render( renderer, posX + columnSpacing[1], posY );

        posX -= columnSpacing[0];

        posY += lineSpacing;
    }

    // Render message
    if ( messageTexture.IsLoaded() )
    {
        posY += 20;
        messageTexture.Render( renderer, posX, posY );
        if ( unrankedScoreTexture.IsLoaded() )
            unrankedScoreTexture.Render( renderer, posX + columnSpacing[0] + columnSpacing[1], posY );
    }
}

void Scoreboard::UpdateCursor()
{
    int timePassed = SDL_GetTicks() - cursorLastTime;
    if ( timePassed >= cursorDelay )
    {
        showCursor = !showCursor;
        cursorLastTime += timePassed;
    }
}

void Scoreboard::RankPlayerScore()
{
    for ( int i = 0; i < listSize; i++ )
    {
        if ( playerScore > scoreList[i] )
        {
            nameList.insert( nameList.begin() + i, playerName );
            nameList.pop_back();
            scoreList.insert( scoreList.begin() + i, playerScore );
            scoreList.pop_back();
            playerRank = i;
            break;
        }
    }
}

void Scoreboard::CharEnter( char c )
{
    if ( playerRank == listSize )
    {
        printf( "Cannot enter character on scoreboard.\n");
        return;
    }


    if ( cursorPos < nameLength )
    {
        if ( c != '.' )
        {
            nameList[playerRank][cursorPos++] = c;
            if ( cursorPos < nameLength )
                nameList[playerRank][cursorPos] = '_';
        }
    }
    else if ( c == '.' )
    {
        playerRank = listSize;
        SaveScoreData( "score.bin" );
    }
}

void Scoreboard::CharBack()
{
    if ( playerRank != listSize && cursorPos != 0 )
    {
        nameList[playerRank][cursorPos--] = ' ';
        nameList[playerRank][cursorPos] = '_';
    }
}
