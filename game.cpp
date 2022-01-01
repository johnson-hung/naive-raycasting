#include <iostream>
#include <chrono>
#include <cmath>
#include <thread>
#include <string>
#include "utils.h"
#include "render.h"
#include "settings.h"
#include "game.h"
#include "SDL.h"
#include "SDL_ttf.h"

enum GameStates{
    STATE_WAITING,
    STATE_RUNNING,
    STATE_TERMINATE,
};
GameStates currentState = STATE_WAITING;

void stateWaiting(SDL_Event& event, std::string& msg, bool& msgFlag){
    if (SDL_PollEvent(&event)){
        if (event.type == SDL_QUIT){
            currentState = STATE_TERMINATE;
            return;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == 'e'){
            std::cout<<"[Game] Change state: Waiting -> Running"<<std::endl;
            msg =  "W - Go Forward     ";
            msg += "S - Go Backward    ";
            msg += "A - Turn Left      ";
            msg += "D - Turn Right     ";
            msg += "Esc - Exit Game    ";
            msgFlag = true;
            currentState = STATE_RUNNING;
        }
    }
}

void stateRunning(SDL_Event& event, Map& map, Player& player){
    // Check user inputs
    if (SDL_PollEvent(&event)){
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)){            
            std::cout<<"[Game] Change state: Running -> Terminate"<<std::endl;
            currentState = STATE_TERMINATE;
            return;
        }
        if (event.type == SDL_KEYUP){
            if (event.key.keysym.sym == 'w' || event.key.keysym.sym == 's') player.walk = 0;
            if (event.key.keysym.sym == 'a' || event.key.keysym.sym == 'd') player.turn = 0;
        }
        if (event.type == SDL_KEYDOWN){
            if (event.key.keysym.sym == 'w') player.walk = 1;
            if (event.key.keysym.sym == 's') player.walk = -1;

            if (event.key.keysym.sym == 'a') player.turn = -1;
            if (event.key.keysym.sym == 'd') player.turn = 1;
        }
    }

    // Update player position and rotation
    float factor = 0.05;
    player.rot += float(player.turn) * factor;
    float diffX = player.walk * cos(player.rot) * factor;
    float diffY = player.walk * sin(player.rot) * factor;
    float nextX = player.x + diffX;
    float nextY = player.y + diffY;

    if ((int) nextX >= 0 && (int) nextX < MAP_WIDTH && (int) nextY >= 0 && (int) nextY < MAP_HEIGHT){
        // Check if there is a block in the next position
        bool posUpdated = false;
        float tol = 0.15;
        float offsetX = diffX >= 0? nextX + tol : nextX - tol;
        float offsetY = diffY >= 0? nextY + tol : nextY - tol;
        if (map.isEmptyAt(offsetX, player.y)){
            player.x = nextX;
            posUpdated = true;
        }
        if (map.isEmptyAt(player.x, offsetY)){
            player.y = nextY;
            posUpdated = true;
        }
        if (posUpdated && (abs(diffX) > 0.01 || abs(diffY) > 0.01)) player.printPlayerPosition();
    }
}

void stateTerminate(bool& terminateFlag){
    std::cout<<"[Game] Terminating..."<<std::endl;
    terminateFlag = true;
}

int main() {
    // Initialize the game
    Game game;
    game.sdlInit();
    game.ttfInit("Press 'E' to start...");
    game.gameInit(
        Canvas(CANVAS_WIDTH, CANVAS_HEIGHT, 0),                             // Window (canvas) properties
        Map(),                                                              // Top-down map
        Player(PLAYER_DEFAULT_X, PLAYER_DEFAULT_Y, 0, PLAYER_DEFAULT_FOV),  //
        Texture(TEXTURE_FILE_WALL, SDL_PIXELFORMAT_ABGR8888),               // Textures of walls
        Texture(TEXTURE_FILE_MONS, SDL_PIXELFORMAT_ABGR8888),               // Textures of monsters
        std::vector<Sprite>{                                                // Sprites of monsters
            (struct Sprite){6.4, 6.2, 2, 0},
            (struct Sprite){4.5, 4.5, 3, 0},
            (struct Sprite){8.0, 8.0, 2, 0},
            (struct Sprite){14.0, 14.2, 0, 0},
            (struct Sprite){12.8, 6.0, 1, 0}
        }
    );

    // State handling (temp)
    bool isTerminated = false;
    bool isTextUpdated = false;
    auto startTime = std::chrono::high_resolution_clock::now();
    std::cout<<"[Game] State: Waiting"<<std::endl;
    while (!isTerminated){
        SDL_Event event;
        // Sleep if the current duration is less than 20 [ms] since last rendering
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = currentTime - startTime; // [ms]
        if (duration.count() < 20){
            std::this_thread::sleep_for(std::chrono::milliseconds(3));
            continue;
        }
        startTime = currentTime;

        // Using this approach to switch game states is not the best way,
        // but it's okay for a small project like this... right? nope.
        switch (currentState){
            case STATE_WAITING:
                stateWaiting(event, game.curText, isTextUpdated);
                break;
            
            case STATE_RUNNING:
                // Check and response to user inputs
                stateRunning(event, game.map, game.player);
                break;
            
            case STATE_TERMINATE:
                stateTerminate(isTerminated);
                break;
        }
        // Update current world and objects data
        render(game.canvas, game.map, game.wallTextures, game.monsterTextures, game.player, game.monsters);

        // Update content and display
        game.ttfUpdate();
        game.sdlUpdate();
        game.render();
    }
    // Clean up
    game.ttfCleanup();
    game.sdlCleanup();
    SDL_Quit();

    return 0;
}