#include "state_terminate.h"

StateTerminate StateTerminate::i_StateTerminate;

void StateTerminate::handleEvents(Game* game){}

void StateTerminate::update(Game* game){
    std::cout<<"[Game] Terminating..."<<std::endl;
    game->terminate();
}
void StateTerminate::render(Game* game){}

StateTerminate* StateTerminate::getInstance(){ return &i_StateTerminate; }
