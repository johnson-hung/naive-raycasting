#ifndef STATE_WAITING_H
#define STATE_WAITING_H

#include "game_state.h"

class StateWaiting: public GameState{
    private:
        static StateWaiting i_StateWaiting;

    protected:
        StateWaiting(){}
    
    public:
        void init(Game* game);
        void handleEvents(Game* game);
        void update(Game* game);
        void render(Game* game);

        static StateWaiting* getInstance();

        ~StateWaiting(){}
};

#endif