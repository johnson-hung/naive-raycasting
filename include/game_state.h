#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "game.h"

class GameState{
    protected:
        GameState(){}

    public:
        virtual void init(Game* game) = 0;
        virtual void handleEvents(Game* game) = 0;
        virtual void update(Game* game) = 0;
        virtual void render(Game* game) = 0;
        void changeState(Game* game, GameState* newState){
            game->changeState(newState);
        }
};

#endif