#include "sprite.h"

bool compareSpriteDistance(Sprite& a, Sprite& b){
    return a.distToPlayer > b.distToPlayer;
}

void updateSpriteDistanceToPlayer(Sprite& sprite, Player& player){
    sprite.distToPlayer = std::sqrt(pow(player.x - sprite.x, 2) + pow(player.y - sprite.y, 2));
}