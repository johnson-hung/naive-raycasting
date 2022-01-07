#include <iostream>
#include <chrono>
#include <cmath>
#include <thread>
#include <string>
#include "settings.h"
#include "game.h"
#include "state_waiting.h"

bool isCurrentWorld(std::chrono::system_clock::time_point& startTime){
    // Sleep if the current duration is less than WORLD_UPDATE_TIME [ms] since last rendering
    auto currentTime = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> duration = currentTime - startTime; // [ms]
    if (duration.count() < WORLD_UPDATE_PERIOD){
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
        return true;
    }
    startTime = currentTime;
    return false;
}

int main() {
    // Initialize the game
    Game game;
    game.sdlInit();
    game.ttfInit();
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
    game.changeState(StateWaiting::getInstance());

    // Game update
    auto startTime = std::chrono::system_clock::now();
    while (game.running()){
        if (isCurrentWorld(startTime)) continue;
        // Update current world and objects data
        game.gameUpdate();

        // Update content and display
        game.sdlUpdate();
        game.ttfUpdate();
        game.sdlRender();
    }
    // Clean up
    game.ttfCleanup();
    game.sdlCleanup();
    SDL_Quit();

    return 0;
}