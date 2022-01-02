#ifndef SPRITE_H
#define SPRITE_H

#include <cstdlib>
#include <cmath>
#include "player.h"

struct Sprite{
    float x;
    float y;
    size_t textureIdx;
    double distToPlayer;
};

// Compare two sprites, sprite with shorter distance to player should be in the front
bool compareSpriteDistance(Sprite& a, Sprite& b);

void updateSpriteDistanceToPlayer(Sprite& sprite, Player& player);

#endif