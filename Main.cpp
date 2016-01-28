#include <stdio.h>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "TetrisClone.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool Main_Init();
void Main_Cleanup();

struct SDLdeleter
{
  void operator()( SDL_Window* window ) const { SDL_DestroyWindow( window ); }
  void operator()( SDL_Renderer* renderer ) const { SDL_DestroyRenderer( renderer ); }
};

std::unique_ptr<SDL_Window, SDLdeleter> mainWindow;
std::unique_ptr<SDL_Renderer, SDLdeleter> mainRenderer;

bool Main_Init()
{
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialise. SDL Error: %s\n", SDL_GetError() );
        return false;
    }

    if ( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1") )
    {
        printf( "Warning: Linear texture filtering could not be enabled." );
    }

    mainWindow.reset( SDL_CreateWindow( "Tetris Clone",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN) );
    if ( mainWindow.get() == NULL )
    {
        printf( "Window could not be created. SDL error: %s", SDL_GetError() );
        return false;
    }

    mainRenderer.reset( SDL_CreateRenderer( mainWindow.get(), -1, SDL_RENDERER_ACCELERATED ) );
    if ( mainRenderer.get() == NULL )
    {
        printf( "Renderer could not be created. SDL error: %s\n", SDL_GetError() );
        return false;
    }

    SDL_SetRenderDrawColor( mainRenderer.get(), 0xFF, 0xFF, 0xFF, 0xFF );

    bool success = true;

    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf("SDL_image could not initialise. SDL_image error: %s\n", IMG_GetError() );
        success = false;
    }

    if( TTF_Init() == -1 )
    {
        printf( "SDL_ttf could not initialise. SDL_ttf error: %s\n", TTF_GetError() );
        success = false;
	}

    return success;
}

void Main_Cleanup()
{
    mainRenderer.reset( nullptr );
    mainWindow.reset( nullptr );

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main( int argc, char *args[] )
{
    if ( !Main_Init() )
    {
        printf( "Failed to initialise.\n" );
        Main_Cleanup();
        return -1;
    }

    TetrisClone game;
    game.Init( mainRenderer.get() );

    unsigned int lastTime = 0;
    unsigned int thisTime;

    while ( game.IsRunning() )
    {
        game.HandleEvents();
        game.Update();

        thisTime = SDL_GetTicks();
        if ( thisTime - lastTime >= 20 )
        {
            game.Render();
            lastTime = thisTime;
        }
    }

    game.Cleanup();

    Main_Cleanup();

    return 0;
}
