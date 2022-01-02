#include "SDL.h"
#include "game.h"
#include "state_waiting.h"
#include "state_running.h"
#include "state_terminate.h"

StateWaiting StateWaiting::i_StateWaiting;

void StateWaiting::handleEvents(Game* game){
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

void StateWaiting::update(Game* game){}

void StateWaiting::render(Game* game){}

StateWaiting* StateWaiting::getInstance(){ return &i_StateWaiting; }
