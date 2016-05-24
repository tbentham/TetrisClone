# TetrisClone
A basic clone of the classic game Tetris, written in C++ using SDL. (2015/16)

## Classes

* **TetrisClone** - General container and controller for the game.
* **GameState** - Superclass for multiple game states.
* **TitleGameState** - Game state where title and main menu are displayed.
* **PlayGameState** - Game state where game is actually played.
* **ScoreGameState** - Game state where scores are recorded and displayed.
* **Menu** - Main menu of options displayed during the title game state.
* **Field** - Handles game blocks during the play game state.
* **Scoreboard** - Handles score data and displays scores during the score game state.
* **Texture** - Wrapper for 'SDL_Texture' class ( based on code by [Lazy Foo' Productions](http://lazyfoo.net/tutorials/SDL/07_texture_loading_and_rendering/index.php) ).
