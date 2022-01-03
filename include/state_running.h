#ifndef STATE_RUNNING_H
#define STATE_RUNNING_H

#include "game_state.h"

class StateRunning: public GameState{
    private:
        static StateRunning i_StateRunning;

    protected:
        StateRunning(){}

    public:
        void init(Game* game);
        void handleEvents(Game* game);
        void update(Game* game);
        void render(Game* game);

        static StateRunning* getInstance();

        ~StateRunning(){}
};

#endif