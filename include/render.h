#ifndef RENDER_H
#define RENDER_H

#include <vector>
#include "canvas.h"
#include "map.h"
#include "texture.h"
#include "player.h"
#include "sprite.h"

namespace Render{
    // Sort sprites by distance in descending order so that far sprites would get rendered first
    bool sortSpritesByDistance(std::vector<Sprite>& sprites, Player& player);

    // Render world environment (ceiling, floor, and blocks) and return buffer of hitpoint distances to player
    std::vector<float> renderWorldEnvironment(Canvas& canvas, Map& map, Texture& textures, Player& player);
    
    // Render a world sprite. Sprite pixel is visible if it's in the range of main screen and not blocked
    bool renderWorldSprite(Canvas& canvas, Sprite& sprite, Texture& texture, Player& player, std::vector<float>& buf);
    
    // Render sorted world sprites (sprites would be sorted by distance in descending order)
    bool renderWorldSprites(Canvas& canvas, Player& player, std::vector<Sprite>& sprites, Texture& spriteTextures, std::vector<float> distBuffer);
    
    // Render 3D view of the current world with environment and sprites included
    bool renderWorld(Canvas& canvas, Map& map, Texture& textures, Texture& spriteTextures, Player& player, std::vector<Sprite>& sprites);
    
    // Draw predefined blocks in the world on the map
    bool renderMapEnvironment(Canvas& canvas, Map& map, Texture& textures);

    // Draw a green dot on the map to show current player position in the world
    bool renderMapPlayer(Canvas& canvas, Player& player);

    // Draw red dots on the map to show current sprite positions in the world
    bool renderMapSprites(Canvas& canvas, std::vector<Sprite>& sprites);

    // Draw a map with environment, player, and sprites included
    bool renderMap(Canvas& canvas, Map& map, Texture& textures, Player& player, std::vector<Sprite>& sprites);
    
    // Draw a field as background to hold a map and text display
    bool renderHUDPlaceholder(Canvas& canvas);

    // Render the current world, HUD placeholder and a map
    void render(Canvas& canvas, Map& map, Texture& textures, Texture& spriteTextures, Player& player, std::vector<Sprite>& sprites);
}

#endif