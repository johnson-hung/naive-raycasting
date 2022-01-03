#ifndef STATE_TERMINATE
#define STATE_TERMINATE

#include "game_state.h"

class StateTerminate: public GameState{
    private:
        static StateTerminate i_StateTerminate;

    protected:
        StateTerminate(){}

    public:
        void init(Game* game);
        void handleEvents(Game* game);
        void update(Game* game);
        void render(Game* game);

        static StateTerminate* getInstance();

        ~StateTerminate(){}
};

#endif