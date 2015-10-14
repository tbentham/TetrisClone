#ifndef SCOREBOARD_H_INCLUDED
#define SCOREBOARD_H_INCLUDED

#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>

#include "Texture.h"

class Scoreboard
{
private:
    static const int listSize = 10;
    static const int nameLength = 3;
    static const int fontSize = 28;
    static const int lineSpacing = 30;
    static const int columnSpacing = 200;
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
    bool PlayerRanked() { return playerRank != listSize; };
};

#endif // SCOREBOARD_H_INCLUDED
