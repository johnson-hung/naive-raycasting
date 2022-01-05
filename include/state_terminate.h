#ifndef STATE_TERMINATE
#define STATE_TERMINATE

#include "game_state.h"

class StateTerminate: public GameState{
    private:
        static StateTerminate i_StateTerminate; // Singleton

    protected:
        StateTerminate(){}

    public:
        // Set up the world or canvas display for the current game state
        void init(Game* game);

        // Handle events (e.g. user inputs)
        void handleEvents(Game* game);

        // Method to update the world under the current game state
        void update(Game* game);
        
        // Method to render the world under the current game state
        void render(Game* game);

        // Return pointer to the singleton of terminate state
        static StateTerminate* getInstance();

        ~StateTerminate(){}
};

#endif