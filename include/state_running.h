#ifndef STATE_RUNNING_H
#define STATE_RUNNING_H

#include "game_state.h"

class StateRunning: public GameState{
    private:
        static StateRunning i_StateRunning; // Singleton

    protected:
        StateRunning(){}

    public:
        // Set up the world or canvas display for the current game state
        void init(Game* game);

        // Handle events (e.g. user inputs)
        void handleEvents(Game* game);

        // Method to update the world under the current game state
        void update(Game* game);

        // Method to render the world under the current game state
        void render(Game* game);

        // Return pointer to the singleton of running state
        static StateRunning* getInstance();

        ~StateRunning(){}
};

#endif