#include <iostream>
#include "player.h"

Player::Player(){
    walk = 0;
    turn = 0;
}

Player::Player(float pX, float pY, float pRot, float pFov){
    x = pX;
    y = pY;
    rot = pRot;
    fov = pFov;
}

void Player::printPlayerPosition(){
    std::cout << "Player: ( " << x <<", " << y << ")" << std::endl;
}