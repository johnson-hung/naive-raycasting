#include <iostream>
#include "player.h"
#include "settings.h"

Player::Player(){
    x = PLAYER_DEFAULT_X;
    y = PLAYER_DEFAULT_Y;
    rot = 0;
    fov = PLAYER_DEFAULT_FOV;
    walk = 0;
    turn = 0;
}

Player::Player(float pX, float pY, float pRot, float pFov){
    x = pX;
    y = pY;
    rot = pRot;
    fov = pFov;
    walk = 0;
    turn = 0;
}

void Player::printPlayerPosition(){
    std::cout << "Player: (" << x <<", " << y << ")" << std::endl;
}