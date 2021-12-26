#include <iostream>
#include <chrono>
#include <cmath>
#include <thread>
#include "utils.h"
#include "canvas.h"
#include "map.h"
#include "texture.h"
#include "player.h"
#include "sprite.h"
#include "raycasting.h"
#include "SDL.h"

int main() {
    // Window (canvas) properties
    Canvas canvas(CANVAS_WIDTH, CANVAS_HEIGHT, packColor(255, 255, 255)); // Initialize the canvas

    // Initialize the map and get properties
    Map map;

    // Initialize the player
    Player player(7.5, 7.5, -120 * PI / 180, PI / 3);
    player.printPlayerPosition();
    
    // Initialize the wall textures
    Texture wallTextures("./textures/wallTextures.png");
    Texture monsterTextures("./textures/monsterTextures.png");
    if (wallTextures.isEmpty() || monsterTextures.isEmpty()){
        std::cerr << "Failed to load wall textures" << std::endl;
        return -1;
    }

    // Initialize monster sprites
    size_t numMonsters = 6;
    std::vector<Sprite> monsters(numMonsters);
    monsters[0] = (struct Sprite){6.4, 6.2, 2, 0};
    monsters[1] = (struct Sprite){4.5, 4.5, 3, 0};
    monsters[2] = (struct Sprite){8.0, 8.0, 2, 0};
    monsters[3] = (struct Sprite){14.0, 14.2, 0, 0};
    monsters[4] = (struct Sprite){12.8, 6.0, 1, 0};
    monsters[4] = (struct Sprite){2.9, 13.5, 2, 0};

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO)){
        std::cerr<<"Failed to initialize SDL: "<<SDL_GetError()<<std::endl;
        return -1;
    }
    if (SDL_CreateWindowAndRenderer(CANVAS_WIDTH, CANVAS_HEIGHT,
                                    SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS, &window, &renderer)){
        std::cerr<<"Failed to create window and renderer: "<<SDL_GetError()<<std::endl;
        return -1;                               
    }
    SDL_Texture *canvasTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888,
                                 SDL_TEXTUREACCESS_STREAMING, CANVAS_WIDTH, CANVAS_HEIGHT);


    auto startTime = std::chrono::high_resolution_clock::now();
    while (1){
        // Sleep if the current duration is less than 20 [ms] since last rendering
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = currentTime - startTime; // [ms]
        if (duration.count() < 20){
            std::this_thread::sleep_for(std::chrono::milliseconds(3));
            continue;
        }
        startTime = currentTime;

        // Check user inputs
        SDL_Event event;
        if (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)){
                break;
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

        // Render current world and objects
        render(canvas, map, wallTextures, monsterTextures, player, monsters);

        // Copy canvas data to the screen
        SDL_UpdateTexture(canvasTexture, NULL, reinterpret_cast<void*>(canvas.getImage().data()), CANVAS_WIDTH*4);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, canvasTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_DestroyTexture(canvasTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}