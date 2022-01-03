#include <iostream>
#include "state_terminate.h"

StateTerminate StateTerminate::i_StateTerminate;

void StateTerminate::init(Game* game){
    std::cout<<"[Game] State: Terminate"<<std::endl;
    game->textRect.w = 205;
    game->curText = "Terminating...";
    game->terminate();
}

void StateTerminate::handleEvents(Game* game){}

void StateTerminate::update(Game* game){}

void StateTerminate::render(Game* game){}

StateTerminate* StateTerminate::getInstance(){ return &i_StateTerminate; }
