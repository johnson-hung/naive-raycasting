#ifndef PLAYER_H
#define PLAYER_H

class Player{
    private:
    
    public: 
        float x;
        float y;
        float rot; // Rotation
        float fov; // Field of View
        int walk; // 0 -> stop; 1 -> forward; -1 -> backward
        int turn; // 0 -> stop; 1 -> rightward; -1 -> leftward

        Player();
        Player(float pX, float pY, float pRot, float pFov);

        void printPlayerPosition();
};

#endif