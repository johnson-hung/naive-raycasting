#ifndef GAME_H
#define GAME_H

#include <vector>
#include "canvas.h"
#include "map.h"
#include "player.h"
#include "texture.h"
#include "sprite.h"
#include "SDL.h"
#include "SDL_ttf.h"

class GameState;

class Game{
    private:
        // Game status
        GameState* state;
        bool isRunning;

    public:
        // Basic elements for running the game
        Canvas canvas;
        Map map;
        Player player;
        Texture wallTextures;
        Texture monsterTextures;
        std::vector<Sprite> monsters;

        // User interface
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* canvasTexture;

        // Text display
        TTF_Font* font;
        std::string preText;
        std::string curText;
        SDL_Rect textRect;
        SDL_Color textColor;
        SDL_Texture* textTexture;
        SDL_Surface* textSurface;

        Game();
        
        // Initialize the game world
        void gameInit(const Canvas& _canvas,
                      const Map& _map,
                      const Player& _player,
                      const Texture& _wallTextures,
                      const Texture& _monsterTextures,
                      const std::vector<Sprite>& _monsters);

        // Update the current world and refresh content display
        void gameUpdate();

        // Initialize SDL window, renderer and texture with predefined CANVAS_WIDTH and CANVAS_HEIGHT
        void sdlInit();

        // Copy canvas data to the SDL renderer
        void sdlUpdate();

        // Destroy SDL texture, renderer, and window
        void sdlCleanup();

        // Render data maintained in the SDL renderer
        void sdlRender();

        // Initialize SDL_ttf (True Type Font) text display
        void ttfInit();

        // Copy new text data to the SDL renderer
        void ttfUpdate();

        // Destroy SDL_ttf text textures
        void ttfCleanup();

        // Change game state to the given new state
        void changeState(GameState* newState);

        // Check if the game is running
        bool running();

        // Terminate the current game
        void terminate();
};

#endif