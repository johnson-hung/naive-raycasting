#include <iostream>
#include <chrono>
#include <cmath>
#include <thread>
#include <string>
#include "utils.h"
#include "canvas.h"
#include "map.h"
#include "texture.h"
#include "player.h"
#include "sprite.h"
#include "render.h"
#include "settings.h"
#include "SDL.h"
#include "SDL_ttf.h"

enum GameStates{
    STATE_WAITING,
    STATE_RUNNING,
    STATE_TERMINATE,
};
GameStates currentState = STATE_WAITING;


void stateWaiting(SDL_Event& event, std::string& msg, bool& msgFlag){
    if (SDL_PollEvent(&event) && event.type == SDL_KEYDOWN && event.key.keysym.sym == 'e'){
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
    // Window (canvas) properties
    Canvas canvas(CANVAS_WIDTH, CANVAS_HEIGHT, 0); // Initialize the canvas

    // Initialize the map and get properties
    Map map;

    // Initialize the player
    Player player(PLAYER_DEFAULT_X, PLAYER_DEFAULT_Y, 0, PLAYER_DEFAULT_FOV);
    player.printPlayerPosition();
    
    // Initialize the wall textures
    Texture wallTextures(TEXTURE_FILE_WALL, SDL_PIXELFORMAT_ABGR8888);
    Texture monsterTextures(TEXTURE_FILE_MONS, SDL_PIXELFORMAT_ABGR8888);
    if (wallTextures.isEmpty() || monsterTextures.isEmpty()){
        std::cerr << "Failed to load wall textures" << std::endl;
        return -1;
    }

    // Initialize monster sprites
    size_t numMonsters = 5;
    std::vector<Sprite> monsters(numMonsters);
    monsters[0] = (struct Sprite){6.4, 6.2, 2, 0};
    monsters[1] = (struct Sprite){4.5, 4.5, 3, 0};
    monsters[2] = (struct Sprite){8.0, 8.0, 2, 0};
    monsters[3] = (struct Sprite){14.0, 14.2, 0, 0};
    monsters[4] = (struct Sprite){12.8, 6.0, 1, 0};

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cerr<<"Failed to initialize SDL: "<<SDL_GetError()<<std::endl;
        return -1;
    }

    if (SDL_CreateWindowAndRenderer(CANVAS_WIDTH, CANVAS_HEIGHT,
                                    SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS, &window, &renderer) != 0){
        std::cerr<<"Failed to create window and renderer: "<<SDL_GetError()<<std::endl;
        return -1;                               
    }
    SDL_Texture* canvasTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888,
                                 SDL_TEXTUREACCESS_STREAMING, CANVAS_WIDTH, CANVAS_HEIGHT);


    if (TTF_Init() != 0){
        std::cerr<<"Failed to create initialize SDL_ttf: "<<TTF_GetError()<<std::endl;
        return -1;
    }
    font = TTF_OpenFont(FONT_FILE, FONT_SIZE);
    if (!font){
        std::cerr<<"Failed to load font: "<<TTF_GetError()<<std::endl;
        return -1;
    }
    std::string text = "Press 'E' to start...";
    SDL_Color textColor = {255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect;
    textRect.x = TEXTFIELD_X;
    textRect.y = TEXTFIELD_Y;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;


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
        // but it's okay for a small project like this... right?
        switch (currentState){
            case STATE_WAITING:
                stateWaiting(event, text, isTextUpdated);
                break;
            
            case STATE_RUNNING:
                // Check and response to user inputs
                stateRunning(event, map, player);
                break;
            
            case STATE_TERMINATE:
                stateTerminate(isTerminated);
                break;
        }

        // Update current world and objects data
        render(canvas, map, wallTextures, monsterTextures, player, monsters);

        // Update text surface before rendering
        if (isTextUpdated){
            textSurface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), textColor, textRect.w);
            textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            textRect.w = textSurface->w;
            textRect.h = textSurface->h;
            isTextUpdated = false;
        }

        // Copy canvas and text data to the screen
        SDL_UpdateTexture(canvasTexture, NULL, reinterpret_cast<void*>(canvas.getImage().data()), CANVAS_WIDTH*4);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, canvasTexture, NULL, NULL);
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(canvasTexture);
    SDL_DestroyTexture(textTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}