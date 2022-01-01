#ifndef GAME_H
#define GAME_H

#include <vector>
#include <iostream>
#include "canvas.h"
#include "map.h"
#include "texture.h"
#include "player.h"
#include "sprite.h"

class GameState;

class Game{
    private:
        // Game status
        std::vector<GameState*> states;
        bool isRunning;

    public:
        // Basic elements for running the game
        Canvas canvas;
        Map map;
        Player player;
        Texture wallTextures;
        Texture monsterTextures;
        std::vector<Sprite> monsters;

        Game(){}

        void init(const Canvas& _canvas,
                  const Map& _map,
                  const Player& _player,
                  const Texture& _wallTextures,
                  const Texture& _monsterTextures,
                  const std::vector<Sprite>& _monsters){
            canvas = _canvas;
            map = _map;
            player = _player;
            player.printPlayerPosition();
            wallTextures = _wallTextures;
            monsterTextures = _monsterTextures;
            if (wallTextures.isEmpty() || monsterTextures.isEmpty()){
                std::cerr << "Failed to load wall textures" << std::endl;
                isRunning = false;
            }
            monsters = _monsters;

            isRunning = true;
        }
        
        void changeState(GameState* state){}
        bool running(){ return isRunning; }
        void terminate(){ isRunning = false; }
};

class GameState{
    public:
        virtual void handleEvents(Game* game) = 0;
        virtual void update(Game* game) = 0;
        virtual void render(Game* game) = 0;
        void changeState(Game* game, GameState* state){
            game->changeState(state);
        }
        virtual ~GameState(){};
};

#endif