#ifndef STATE_WAITING_H
#define STATE_WAITING_H

#include "game_state.h"

class StateWaiting: public GameState{
    private:
        static StateWaiting i_StateWaiting; // Singleton

    protected:
        StateWaiting(){}
    
    public:
        // Set up the world or canvas display for the current game state
        void init(Game* game);

        // Handle events (e.g. user inputs)
        void handleEvents(Game* game);

        // Method to update the world under the current game state
        void update(Game* game);

        // Method to render the world under the current game state
        void render(Game* game);

        // Return pointer to the singleton of waiting state
        static StateWaiting* getInstance();

        ~StateWaiting(){}
};

#endif