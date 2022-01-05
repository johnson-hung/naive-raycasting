#include <cmath>
#include "player.h"
#include "sprite.h"

// Compare two sprites, sprite with shorter distance to player should be in the front
bool compareSpriteDistance(Sprite& a, Sprite& b){
    return a.distToPlayer > b.distToPlayer;
}

void updateSpriteDistanceToPlayer(Sprite& sprite, Player& player){
    sprite.distToPlayer = std::sqrt(pow(player.x - sprite.x, 2) + pow(player.y - sprite.y, 2));
}