#include <iostream>
#include "state_terminate.h"

StateTerminate StateTerminate::i_StateTerminate;

// Set up the world or canvas display for the current game state
void StateTerminate::init(Game* game){
    std::cout<<"[Game] State: Terminate"<<std::endl;
    game->textRect.w = 205;
    game->curText = "Terminating...";
    game->terminate();
}

// Handle events (e.g. user inputs)
void StateTerminate::handleEvents(Game* game){}

// Method to update the world under the current game state
void StateTerminate::update(Game* game){}

// Method to render the world under the current game state
void StateTerminate::render(Game* game){}

// Return pointer to the singleton of terminate state
StateTerminate* StateTerminate::getInstance(){ return &i_StateTerminate; }
