#include <iostream>
#include <fstream>
#include <cstdint> // Fixed width integer types 
#include <cassert> // Error handling
#include <cmath>
#include <vector>
#include <algorithm>
#include "settings.h"
#include "utils.h"
#include "render.h"

// Sort sprites by distance in descending order so that far sprites would get rendered first
bool Render::sortSpritesByDistance(std::vector<Sprite>& sprites, Player& player){
    for (size_t i = 0; i < sprites.size(); i++){
        updateSpriteDistanceToPlayer(sprites[i], player);
    }
    std::sort(sprites.begin(), sprites.end(), compareSpriteDistance);
    return true;
}

// Render world environment (ceiling, floor, and blocks) and return buffer of hitpoint distances to player
std::vector<float> Render::renderWorldEnvironment(Canvas& canvas, Map& map, Texture& textures, Player& player){
    canvas.drawRectangle(0, 0, MAIN_WIDTH, MAIN_HEIGHT/2, packColor(40, 40, 40));             // Ceiling
    canvas.drawRectangle(0, MAIN_HEIGHT/2, MAIN_WIDTH, MAIN_HEIGHT/2, packColor(20, 20, 20)); // Floor
    
    const size_t textureCount = textures.getCount();
    const size_t textureSize = textures.getSize();

    // Cache closest distance between player and the block at x = i
    std::vector<float> distBuffer(MAIN_WIDTH, 1e3);

    for(size_t i = 0; i < MAIN_WIDTH; i++){
        float rotation = player.rot - player.fov / 2 + player.fov * (i/(float)MAIN_WIDTH);
        
        // Cast single ray in certain direction
        for (float dist = 0; dist < 23; dist += 0.01){
            float targetX = player.x + dist * cos(rotation);
            float targetY = player.y + dist * sin(rotation);

            // Ray hits a block, render vertical column for 3D view
            if (!map.isEmptyAt((int) targetX, (int) targetY)){
                size_t textureIdx = map.getValueAt((int) targetX, (int) targetY);
                assert(textureIdx < textureCount);
                
                float distFixed = dist * cos(rotation - player.rot); // Fix fisheye distortion
                distBuffer[i] = distFixed;
                size_t h = MAIN_HEIGHT / distFixed; // Height of the block

                // Get fractional part of targetX and targetY to determine the 
                // position of the hitpoint (top/bottom or left/right side of block)
                float hitX = targetX - floor(targetX + 0.5); // ~0 -> hit left/right side of block
                float hitY = targetY - floor(targetY + 0.5); // ~0 -> hit top/bottom side of block
                int start = std::abs(hitY) > std::abs(hitX)? hitY * textureSize : hitX * textureSize;
                if (start < 0) start += textureSize; // Make sure start position is positive
                assert(start >= 0 && start < (int) textureSize);

                // Write the specific column
                std::vector<uint32_t> col = textures.getTextureColumn(textureIdx, start, h);
                int x = i;
                int y = MAIN_HEIGHT/2 - h/2;
                for (size_t j = 0; j < h; j++){
                    y = MAIN_HEIGHT/2 - h/2 + j;
                    if (x < 0 || y >= (int) MAIN_HEIGHT) continue;
                    canvas.drawPixel(x, y, col[j]);
                }
                break;
            }
        }
    }
    return distBuffer;
}

// Render a world sprite. Sprite pixel is visible if it's in the range of main screen and not blocked
bool Render::renderWorldSprite(Canvas& canvas, 
                               Sprite& sprite, 
                               Texture& texture, 
                               Player& player, 
                               std::vector<float>& buf){ // Depth buffer
    // View player as center, get direction of the sprite 
    float spriteDir = atan2(sprite.y - player.y, sprite.x - player.x);

    // Recalculate spriteDir to let it be in range [-PI, PI]
    while (spriteDir - player.rot > PI) spriteDir -= 2*PI;
    while (spriteDir - player.rot < -PI) spriteDir += 2*PI;

    // Calculate distance between the player and the sprite, then get displayed sprite size
    size_t spriteSize = std::min(1000, static_cast<int>(MAIN_HEIGHT/sprite.distToPlayer));

    // Calculate (x, y) position to start rendering
    int startX = (spriteDir-player.rot)/player.fov * MAIN_WIDTH + MAIN_WIDTH/2 - texture.getSize()/2;
    int startY = MAIN_HEIGHT/2 - spriteSize/2;
    for (size_t i = 0; i < spriteSize; i++){
        if (startX + (int)i < 0 || startX + (int)i >= MAIN_WIDTH) continue;
        if (buf[startX + i] < sprite.distToPlayer) continue; // Current column is blocked
        for (size_t j = 0; j < spriteSize; j++){
            if (startY + (int)j < 0 || startY + (int)j >= MAIN_HEIGHT) continue;
            float scale = (float) texture.getSize() / spriteSize;
            uint32_t color = texture.getValueAt(sprite.textureIdx, i * scale, j * scale);
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
            unpackColor(color, r, g, b, a);
            if (a == 255){
                canvas.drawPixel(startX + i, startY + j, color);
            }
        }
    }
    return true;
}

// Render sorted world sprites (sprites would be sorted by distance in descending order)
bool Render::renderWorldSprites(Canvas& canvas,
                                Player& player,
                                std::vector<Sprite>& sprites,
                                Texture& spriteTextures,
                                std::vector<float> distBuffer){
    assert(sortSpritesByDistance(sprites, player));

    for (size_t i = 0; i < sprites.size(); i++){
        assert(renderWorldSprite(canvas, sprites[i], spriteTextures, player, distBuffer));
    } 
    return true;
}

// Render 3D view of the current world with environment and sprites included
bool Render::renderWorld(Canvas& canvas,
                         Map& map,
                         Texture& textures,
                         Texture& spriteTextures,
                         Player& player,
                         std::vector<Sprite>& sprites){
    std::vector<float> distBuffer = renderWorldEnvironment(canvas, map, textures, player);
    renderWorldSprites(canvas, player, sprites, spriteTextures, distBuffer);
    return true;
}

// Draw a green dot on the map to show current player position in the world
bool Render::renderMapPlayer(Canvas& canvas, Player& player){
    if (!MAP_DISPLAY) return true;

    // Draw player on the top-down map
    canvas.drawRectangle(HUD_SHIFT_X + player.x * MAP_RECT_WIDTH,
                         HUD_SHIFT_Y + player.y * MAP_RECT_HEIGHT,5, 5, packColor(0, 255, 0));
    return true;
}

// Draw red dots on the map to show current sprite positions in the world
bool Render::renderMapSprites(Canvas& canvas, std::vector<Sprite>& sprites){
    if (!MAP_DISPLAY) return true;

    for (size_t i = 0; i < sprites.size(); i++){
        canvas.drawRectangle(HUD_SHIFT_X + sprites[i].x * MAP_RECT_WIDTH,
                             HUD_SHIFT_Y + sprites[i].y * MAP_RECT_HEIGHT, 5, 5, packColor(255, 0, 0));
    }
    return true;
}

// Draw predefined blocks in the world on the map
bool Render::renderMapEnvironment(Canvas& canvas, Map& map, Texture& textures){
    if (!MAP_DISPLAY) return true;

    const size_t textureCount = textures.getCount();
    for (size_t x = 0; x < MAP_WIDTH; x++){
        for (size_t y = 0; y < MAP_HEIGHT; y++){
            size_t imgX = HUD_SHIFT_X + x * MAP_RECT_WIDTH;
            size_t imgY = HUD_SHIFT_Y + y * MAP_RECT_HEIGHT;
            if (map.isEmptyAt(x, y)){
                canvas.drawRectangle(imgX, imgY, MAP_RECT_WIDTH, MAP_RECT_HEIGHT, packColor(0, 0, 0));
                continue;
            }

            // Make sure we can find the corresponding texture with given index
            size_t textureIdx = map.getValueAt(x, y);
            assert(textureIdx < textureCount);

            // Pick a pixel to represent the block
            canvas.drawRectangle(imgX, imgY, MAP_RECT_WIDTH, MAP_RECT_HEIGHT, textures.getValueAt(textureIdx, 0, 0));
        }
    }
    return true;
}

// Draw a map with environment, player, and sprites included
bool Render::renderMap(Canvas& canvas, Map& map, Texture& textures, Player& player, std::vector<Sprite>& sprites){
    if (!MAP_DISPLAY) return true;

    assert(renderMapEnvironment(canvas, map, textures));
    assert(renderMapPlayer(canvas, player));
    assert(renderMapSprites(canvas, sprites));
    return true;
}

// Draw a field as background to hold a map and text display
bool Render::renderHUDPlaceholder(Canvas& canvas){
    if (!HUD_DISPLAY) return true;

    canvas.drawRectangle(HUD_SHIFT_X, HUD_SHIFT_Y, HUD_WIDTH, HUD_HEIGHT, packColor(50, 50, 50));
    return true;
}

// Render the current world, HUD placeholder and a map
void Render::render(Canvas& canvas,
                    Map& map, 
                    Texture& textures, 
                    Texture& spriteTextures, 
                    Player& player, 
                    std::vector<Sprite>& sprites){
    canvas.clearCanvas(packColor(255, 255, 255));

    assert(renderWorld(canvas, map, textures, spriteTextures, player, sprites));
    assert(renderHUDPlaceholder(canvas));
    assert(renderMap(canvas, map, textures, player, sprites));
}