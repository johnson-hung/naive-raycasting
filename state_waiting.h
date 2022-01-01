#ifndef STATE_WAITING_H
#define STATE_WAITING_H

#include "game_state.h"
#include "state_running.h"
#include "state_terminate.h"

class StateWaiting: public GameState{
    private:
        static StateWaiting i_StateWaiting;

    protected:
        StateWaiting(){}
    
    public:
        void handleEvents(Game* game){
            SDL_Event event;
            if (SDL_PollEvent(&event)){
                if (event.type == SDL_QUIT){
                    changeState(game, StateTerminate::getInstance());
                    return;
                }
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == 'e'){
                    std::cout<<"[Game] Change state: Waiting -> Running"<<std::endl;
                    game->curText =  "W - Go Forward     ";
                    game->curText += "S - Go Backward    ";
                    game->curText += "A - Turn Left      ";
                    game->curText += "D - Turn Right     ";
                    game->curText += "Esc - Exit Game    ";
                    changeState(game, StateRunning::getInstance());
                }
            }
        }

        void update(Game* game){}

        void render(Game* game){}

        static StateWaiting* getInstance(){ return &i_StateWaiting; }

        ~StateWaiting(){}
};

StateWaiting StateWaiting::i_StateWaiting;

#endif