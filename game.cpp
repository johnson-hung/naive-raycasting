#include <iostream>
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
    
    // Initialize the wall textures
    Texture wallTextures("./textures/wallTextures.png");
    Texture monsterTextures("./textures/monsterTextures.png");
    if (wallTextures.isEmpty() || monsterTextures.isEmpty()){
        std::cerr << "Failed to load wall textures" << std::endl;
        return -1;
    }

    // Initialize monster sprites
    size_t numMonsters = 3;
    std::vector<Sprite> monsters(numMonsters);
    monsters[0] = (struct Sprite){6.4, 6.2, 2, 0};
    monsters[1] = (struct Sprite){4.5, 4.5, 3, 0};
    monsters[2] = (struct Sprite){8.0, 8.0, 2, 0};

    // Render current world and objects
    render(canvas, map, wallTextures, monsterTextures, player, monsters);

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
    SDL_UpdateTexture(canvasTexture, NULL, reinterpret_cast<void*>(canvas.getImage().data()), CANVAS_WIDTH*4);

    SDL_Event event;
    while (1){
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT){
            break;
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, canvasTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(canvasTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}