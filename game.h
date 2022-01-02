#ifndef GAME_H
#define GAME_H

#include <memory>
#include <vector>
#include <iostream>
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
        void gameInit(const Canvas& _canvas,
                      const Map& _map,
                      const Player& _player,
                      const Texture& _wallTextures,
                      const Texture& _monsterTextures,
                      const std::vector<Sprite>& _monsters);
        void gameUpdate();

        void sdlInit();
        void sdlUpdate();
        void sdlCleanup();
        void sdlRender();

        void ttfInit(std::string _text);
        void ttfUpdate();
        void ttfCleanup();

        void changeState(GameState* newState);
        bool running();
        void terminate();
};

#endif