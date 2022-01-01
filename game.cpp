#include <iostream>
#include <chrono>
#include <cmath>
#include <thread>
#include <string>
#include "game.h"
#include "state_waiting.h"

static StateWaiting* i_StateWaiting =  StateWaiting::getInstance();

Game::Game(){
    std::cout<<"Game instance created"<<std::endl;
}

void Game::gameInit(const Canvas& _canvas,
                    const Map& _map,
                    const Player& _player,
                    const Texture& _wallTextures,
                    const Texture& _monsterTextures,
                    const std::vector<Sprite>& _monsters){
    canvas = _canvas;
    map = _map;
    player = _player;
    player.printPlayerPosition();
    wallTextures = _wallTextures;
    monsterTextures = _monsterTextures;
    if (wallTextures.isEmpty() || monsterTextures.isEmpty()){
        std::cerr << "Failed to load wall textures" << std::endl;
        isRunning = false;
    }
    monsters = _monsters;
    isRunning = true;
    std::cout<<"World initialized"<<std::endl;
}

void Game::gameUpdate(){
    state->handleEvents(this);
    state->update(this);
    state->render(this);
    render(canvas, map, wallTextures, monsterTextures, player, monsters);
}

bool isCurrentWorld(std::chrono::steady_clock::time_point& startTime){
    // Sleep if the current duration is less than WORLD_UPDATE_TIME [ms] since last rendering
    auto currentTime = std::chrono::high_resolution_clock::now();
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
    game.changeState(StateWaiting::getInstance());

    // State handling (temp)
    auto startTime = std::chrono::high_resolution_clock::now();
    std::cout<<"[Game] State: Waiting"<<std::endl;
    while (game.running()){
        if (isCurrentWorld(startTime)) continue;
        // Update current world and objects data
        game.gameUpdate();

        // Update content and display
        game.ttfUpdate();
        game.sdlUpdate();
        game.sdlRender();
    }
    // Clean up
    game.ttfCleanup();
    game.sdlCleanup();
    SDL_Quit();

    return 0;
}