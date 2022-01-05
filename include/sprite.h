#ifndef SPRITE_H
#define SPRITE_H

#include <cstdlib>
#include "player.h"

struct Sprite{
    float x;                // X-coordinate according to the map
    float y;                // Y-coordinate according to the map
    size_t textureIdx;      // The index for finding corresponding texture in the texture file
    double distToPlayer;
};

// Compare two sprites, sprite with shorter distance to player should be in the front
bool compareSpriteDistance(Sprite& a, Sprite& b);

void updateSpriteDistanceToPlayer(Sprite& sprite, Player& player);

#endif