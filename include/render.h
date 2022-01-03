#ifndef RENDER_H
#define RENDER_H

#include <vector>
#include "canvas.h"
#include "map.h"
#include "texture.h"
#include "player.h"
#include "sprite.h"

namespace Render{
    bool sortSpritesByDistance(std::vector<Sprite>& sprites, Player& player);

    std::vector<float> renderWorldEnvironment(Canvas& canvas, Map& map, Texture& textures, Player& player);
    bool renderWorldSprite(Canvas& canvas, Sprite& sprite, Texture& texture, Player& player, std::vector<float>& buf);
    bool renderWorldSprites(Canvas& canvas, Player& player, std::vector<Sprite>& sprites, Texture& spriteTextures, std::vector<float> distBuffer);
    bool renderWorld(Canvas& canvas, Map& map, Texture& textures, Texture& spriteTextures, Player& player, std::vector<Sprite>& sprites);
    
    bool renderMapEnvironment(Canvas& canvas, Map& map, Texture& textures);
    bool renderMapPlayer(Canvas& canvas, Player& player);
    bool renderMapSprites(Canvas& canvas, std::vector<Sprite>& sprites);
    bool renderMap(Canvas& canvas, Map& map, Texture& textures, Player& player, std::vector<Sprite>& sprites);
    
    bool renderHUDPlaceholder(Canvas& canvas);
    void render(Canvas& canvas, Map& map, Texture& textures, Texture& spriteTextures, Player& player, std::vector<Sprite>& sprites);
}

#endif