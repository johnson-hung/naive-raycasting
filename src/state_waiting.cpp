#include <iostream>
#include "SDL.h"
#include "render.h"
#include "game.h"
#include "state_waiting.h"
#include "state_running.h"
#include "state_terminate.h"

StateWaiting StateWaiting::i_StateWaiting;

// Set up the world or canvas display for the current game state
void StateWaiting::init(Game* game){
    std::cout<<"[Game] State: Waiting"<<std::endl;
    game->textRect.w = 205;
    game->curText = "Press 'E' to start...";
}

// Handle events (e.g. user inputs)
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

// Method to update the world under the current game state
void StateWaiting::update(Game* game){
    // Do something here...
}

// Method to render the world under the current game state
void StateWaiting::render(Game* game){
    Render::renderWorldEnvironment(game->canvas, game->map, game->wallTextures, game->player);
    Render::renderHUDPlaceholder(game->canvas);
    Render::renderMapEnvironment(game->canvas, game->map, game->wallTextures);
}

// Return pointer to the singleton of waiting state
StateWaiting* StateWaiting::getInstance(){
    return &i_StateWaiting;
}
