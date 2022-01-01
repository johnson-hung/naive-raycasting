#ifndef STATE_TERMINATE
#define STATE_TERMINATE

#include "game_state.h"

class StateTerminate: public GameState{
    private:
        static StateTerminate i_StateTerminate;

    protected:
        StateTerminate(){}

    public:
        void handleEvents(Game* game){}
        void update(Game* game){
            std::cout<<"[Game] Terminating..."<<std::endl;
            game->terminate();
        }
        void render(Game* game){}

        static StateTerminate* getInstance(){ return &i_StateTerminate; }

        ~StateTerminate(){}
};

StateTerminate StateTerminate::i_StateTerminate;

#endif