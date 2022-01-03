#include <iostream>
#include "render.h"
#include "state_running.h"
#include "state_terminate.h"

StateRunning StateRunning::i_StateRunning;

void StateRunning::init(Game* game){
    std::cout<<"[Game] State: Running"<<std::endl;
    game->textRect.w = 205;
    game->curText =  "W - Go Forward     ";
    game->curText += "S - Go Backward    ";
    game->curText += "A - Turn Left      ";
    game->curText += "D - Turn Right     ";
    game->curText += "Esc - Exit Game    ";
}

void StateRunning::handleEvents(Game* game){
    SDL_Event event;
    if (SDL_PollEvent(&event)){
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)){            
            std::cout<<"[Game] Change state: Running -> Terminate"<<std::endl;
            changeState(game, StateTerminate::getInstance());
            return;
        }
        if (event.type == SDL_KEYUP){
            if (event.key.keysym.sym == 'w' || event.key.keysym.sym == 's') game->player.walk = 0;
            if (event.key.keysym.sym == 'a' || event.key.keysym.sym == 'd') game->player.turn = 0;
        }
        if (event.type == SDL_KEYDOWN){
            if (event.key.keysym.sym == 'w') game->player.walk = 1;
            if (event.key.keysym.sym == 's') game->player.walk = -1;

            if (event.key.keysym.sym == 'a') game->player.turn = -1;
            if (event.key.keysym.sym == 'd') game->player.turn = 1;
        }
    }
}

void StateRunning::update(Game* game){
    // Update player position and rotation
    float factor = 0.05;
    game->player.rot += float(game->player.turn) * factor;
    float diffX = game->player.walk * cos(game->player.rot) * factor;
    float diffY = game->player.walk * sin(game->player.rot) * factor;
    float nextX = game->player.x + diffX;
    float nextY = game->player.y + diffY;

    if ((int) nextX >= 0 && (int) nextX < MAP_WIDTH && (int) nextY >= 0 && (int) nextY < MAP_HEIGHT){
        // Check if there is a block in the next position
        bool posUpdated = false;
        float tol = 0.15;
        float offsetX = diffX >= 0? nextX + tol : nextX - tol;
        float offsetY = diffY >= 0? nextY + tol : nextY - tol;
        if (game->map.isEmptyAt(offsetX, game->player.y)){
            game->player.x = nextX;
            posUpdated = true;
        }
        if (game->map.isEmptyAt(game->player.x, offsetY)){
            game->player.y = nextY;
            posUpdated = true;
        }
        if (posUpdated && (abs(diffX) > 0.01 || abs(diffY) > 0.01)) game->player.printPlayerPosition();
    }
}

void StateRunning::render(Game* game){
    Render::render(game->canvas, game->map, game->wallTextures, game->monsterTextures, game->player, game->monsters);
}

StateRunning* StateRunning::getInstance(){ return &i_StateRunning; }
