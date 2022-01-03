#include <iostream>
#include "SDL.h"
#include "render.h"
#include "game.h"
#include "state_waiting.h"
#include "state_running.h"
#include "state_terminate.h"

StateWaiting StateWaiting::i_StateWaiting;

void StateWaiting::init(Game* game){
    std::cout<<"[Game] State: Waiting"<<std::endl;
    game->textRect.w = 205;
    game->curText = "Press 'E' to start...";
}

void StateWaiting::handleEvents(Game* game){
    SDL_Event event;
    if (SDL_PollEvent(&event)){
        if (event.type == SDL_QUIT){
            std::cout<<"[Game] Change state: Waiting -> Terminate"<<std::endl;
            changeState(game, StateTerminate::getInstance());
            return;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == 'e'){
            std::cout<<"[Game] Change state: Waiting -> Running"<<std::endl;
            changeState(game, StateRunning::getInstance());
        }
    }
}

void StateWaiting::update(Game* game){}

void StateWaiting::render(Game* game){
    Render::render(game->canvas, game->map, game->wallTextures, game->monsterTextures, game->player, game->monsters);
}

StateWaiting* StateWaiting::getInstance(){ return &i_StateWaiting; }
