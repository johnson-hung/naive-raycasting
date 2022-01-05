#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "game.h"

class GameState{
    protected:
        GameState(){}

    public:
        // Set up the world for the current game state
        virtual void init(Game* game) = 0;

        // Handle events (e.g. user inputs)
        virtual void handleEvents(Game* game) = 0;

        // Method to update the world under the current game state
        virtual void update(Game* game) = 0;

        // Method to render the world under the current game state
        virtual void render(Game* game) = 0;

        // Pass new state to game manager to switch state
        void changeState(Game* game, GameState* newState){
            game->changeState(newState);
        }
};

#endif