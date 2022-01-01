#ifndef GAME_H
#define GAME_H

#include <vector>
#include <iostream>
#include "canvas.h"
#include "map.h"
#include "texture.h"
#include "player.h"
#include "sprite.h"
#include "SDL.h"
#include "SDL_ttf.h"

class GameState;

class Game{
    private:
        // Game status
        std::vector<GameState*> states;
        bool isRunning;

    public:
        // Basic elements for running the game
        Canvas canvas;
        Map map;
        Player player;
        Texture wallTextures;
        Texture monsterTextures;
        std::vector<Sprite> monsters;

        // User interface
        SDL_Window* window          = nullptr;
        SDL_Renderer* renderer      = nullptr;
        SDL_Texture* canvasTexture  = nullptr;

        // Text display
        TTF_Font* font              = nullptr;
        std::string preText;
        std::string curText;
        SDL_Rect textRect;
        SDL_Color textColor = {255, 255, 255};
        SDL_Texture* textTexture    = nullptr;
        SDL_Surface* textSurface    = nullptr;

        Game(){
            std::cout<<"Game instance created"<<std::endl;
        }

        void gameInit(const Canvas& _canvas,
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


        void sdlInit(){
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

        void sdlUpdate(){
            // Copy canvas and text data to the screen
            SDL_UpdateTexture(canvasTexture, NULL, reinterpret_cast<void*>(canvas.getImage().data()), CANVAS_WIDTH*4);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, canvasTexture, NULL, NULL);
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        }
        
        void sdlCleanup(){
            SDL_DestroyTexture(canvasTexture);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            std::cout<<"SDL cleaned"<<std::endl;
        }


        void ttfInit(std::string _text){
            curText = _text;
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

        void ttfUpdate(){
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

        void ttfCleanup(){
            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
            std::cout<<"SDL_TTF cleaned"<<std::endl;
        }


        void render(){ SDL_RenderPresent(renderer); }
        void changeState(GameState* state){}
        bool running(){ return isRunning; }
        void terminate(){ isRunning = false; }
};

class GameState{
    public:
        virtual void handleEvents(Game* game) = 0;
        virtual void update(Game* game) = 0;
        virtual void render(Game* game) = 0;
        void changeState(Game* game, GameState* state){
            game->changeState(state);
        }
        virtual ~GameState(){};
};

#endif