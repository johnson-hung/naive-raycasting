#ifndef PLAYER_H
#define PLAYER_H

class Player{
    private:
    
    public: 
        float x;
        float y;
        float rot; // Rotation
        float fov; // Field of View

        Player(float pX, float pY, float pRot, float pFov);
};

#endif