#include <iostream>
#include "settings.h"
#include "game.h"
#include "game_state.h"
#include "SDL.h"
#include "SDL_ttf.h"

Game::Game(){
    state = nullptr;
    isRunning = false;

    window = nullptr;
    renderer = nullptr;
    canvasTexture = nullptr;

    font = nullptr;
    preText = "";
    curText = "";
    textColor = {255, 255, 255};
    textTexture = nullptr;
    textSurface = nullptr;
    std::cout<<"Game instance created"<<std::endl;
}

// Initialize the game world
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

// Update the current world and refresh content display
void Game::gameUpdate(){
    state->handleEvents(this);
    state->update(this);
    state->render(this);
}

// Initialize SDL window, renderer and texture with predefined CANVAS_WIDTH and CANVAS_HEIGHT
void Game::sdlInit(){
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cerr<<"Failed to initialize SDL: "<<SDL_GetError()<<std::endl;
        return;
    }

    if (SDL_CreateWindowAndRenderer(CANVAS_WIDTH, CANVAS_HEIGHT,
                                    SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS, &window, &renderer) != 0){
        std::cerr<<"Failed to create window and renderer: "<<SDL_GetError()<<std::endl;
        return;                               
    }
    canvasTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING,
                                        CANVAS_WIDTH, CANVAS_HEIGHT);
    
    std::cout<<"SDL initialized"<<std::endl;
}

// Copy canvas data to the SDL renderer
void Game::sdlUpdate(){
    SDL_UpdateTexture(canvasTexture, NULL, reinterpret_cast<void*>(canvas.getImage().data()), CANVAS_WIDTH*4);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, canvasTexture, NULL, NULL);
}

// Destroy SDL texture, renderer, and window
void Game::sdlCleanup(){
    SDL_DestroyTexture(canvasTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    std::cout<<"SDL cleaned"<<std::endl;
}

// Render data maintained in the SDL renderer
void Game::sdlRender(){
    SDL_RenderPresent(renderer);
}

// Initialize SDL_ttf (True Type Font) text display
void Game::ttfInit(){
    curText = "Initializing...";
    preText = curText;
    if (TTF_Init() != 0){
        std::cerr<<"Failed to create initialize SDL_ttf: "<<TTF_GetError()<<std::endl;
        return;
    }
    font = TTF_OpenFont(FONT_FILE, FONT_SIZE);
    if (!font){
        std::cerr<<"Failed to load font: "<<TTF_GetError()<<std::endl;
        return;
    }

    textSurface = TTF_RenderText_Solid(font, curText.c_str(), textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    textRect.x = TEXTFIELD_X;
    textRect.y = TEXTFIELD_Y;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;
    std::cout<<"SDL_ttf initialized"<<std::endl;
}

// Copy new text data to the SDL renderer
void Game::ttfUpdate(){
    if (curText != preText){
        textSurface = TTF_RenderText_Blended_Wrapped(font, curText.c_str(), textColor, textRect.w);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        textRect.w = textSurface->w;
        textRect.h = textSurface->h;
        preText = curText;
    }
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
}

// Destroy SDL_ttf text textures
void Game::ttfCleanup(){
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    std::cout<<"SDL_ttf cleaned"<<std::endl;
}


// Change game state to the given new state
void Game::changeState(GameState* newState){
    std::cout<<"Game state changed"<<std::endl;
    state = newState;
    state->init(this);
}

// Check if the game is running
bool Game::running(){
    return isRunning;
}

// Terminate the current game
void Game::terminate(){
    isRunning = false;
}