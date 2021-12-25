#ifndef SPRITE_H
#define SPRITE_H

#include <cstdlib>

struct Sprite{
    float x;
    float y;
    size_t textureIdx;
    double distToPlayer;
};

// Compare two sprites, sprite with shorter distance to player should be in the front
bool compareSpriteDistance(Sprite& a, Sprite& b){
    return a.distToPlayer > b.distToPlayer;
}

#endif