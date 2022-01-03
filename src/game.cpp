#include <iostream>
#include <memory>
#include "render.h"
#include "game.h"
#include "state_waiting.h"

static StateWaiting* i_StateWaiting =  StateWaiting::getInstance();

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

void Game::sdlUpdate(){
    // Copy canvas and text data to the screen
    SDL_UpdateTexture(canvasTexture, NULL, reinterpret_cast<void*>(canvas.getImage().data()), CANVAS_WIDTH*4);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, canvasTexture, NULL, NULL);
}

void Game::sdlCleanup(){
    SDL_DestroyTexture(canvasTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    std::cout<<"SDL cleaned"<<std::endl;
}


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
    std::cout<<"SDL_TTF initialized"<<std::endl;
}

void Game::ttfUpdate(){
    // Update text surface before rendering
    if (curText != preText){
        textSurface = TTF_RenderText_Blended_Wrapped(font, curText.c_str(), textColor, textRect.w);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        textRect.w = textSurface->w;
        textRect.h = textSurface->h;
        preText = curText;
    }
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
}

void Game::ttfCleanup(){
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    std::cout<<"SDL_TTF cleaned"<<std::endl;
}

void Game::sdlRender(){ SDL_RenderPresent(renderer); }

void Game::changeState(GameState* newState){
    std::cout<<"Game state changed"<<std::endl;
    state = newState;
    state->init(this);
}

bool Game::running(){ return isRunning; }
void Game::terminate(){ isRunning = false; }