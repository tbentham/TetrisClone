#ifndef SCOREBOARD_H_INCLUDED
#define SCOREBOARD_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>

#include "Texture.h"

/* Handles score data and displays scores during the score game state */
class Scoreboard
{
private:
    static const int listSize = 10;
    static const int nameLength = 3;
    static const int fontSize = 28;
    static const int lineSpacing = 30;
    const int columnSpacing[2] = { 50, 200 };
    static const int cursorDelay = 200;

    std::vector<std::string> nameList;
    std::vector<int> scoreList;

    std::string playerName;
    int playerScore;
    int playerRank; // 0-indexed
    int cursorPos;
    bool showCursor;
    int cursorLastTime;

    Texture arrowTexture;

    TTF_Font* font;
    std::vector<Texture> charTextures;
    std::vector<Texture> numTextures;
    std::vector<Texture> scoreTextures;
    Texture messageTexture;
    Texture unrankedScoreTexture;
public:
    Scoreboard();
    ~Scoreboard();

    bool LoadScoreData( const char* fileLocation );
    bool SaveScoreData( const char* fileLocation );
    bool LoadTextures( SDL_Renderer* renderer );

    void Render( SDL_Renderer* renderer, int x, int y );

    void UpdateCursor();

    void RankPlayerScore();

    void CharEnter( char c );
    void CharBack();

    int GetPlayerScore() { return playerScore; };
    void SetPlayerScore( int score ) { playerScore = score; };
    bool PlayerBeingRanked() { return playerRank != listSize; };
};

#endif // SCOREBOARD_H_INCLUDED
