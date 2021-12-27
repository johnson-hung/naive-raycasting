#ifndef RAYCASTING_H
#define RAYCASTING_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint> // Fixed width integer types 
#include <cassert> // Error handling
#include <cmath>
#include <algorithm>
#include "utils.h"
#include "canvas.h"
#include "map.h"
#include "texture.h"
#include "player.h"
#include "sprite.h"
#include "settings.h"

bool sortSpritesByDistance(std::vector<Sprite>& sprites, Player& player){
    for (size_t i = 0; i < sprites.size(); i++){
        updateSpriteDistanceToPlayer(sprites[i], player);
    }
    std::sort(sprites.begin(), sprites.end(), compareSpriteDistance);
    return true;
}

bool renderWorldSprite(Canvas& canvas, 
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

// Cast field of view on the top-down map and 3D view
bool renderWorld(Canvas& canvas,
                 Map& map,
                 Texture& textures,
                 Texture& spriteTextures,
                 Player& player,
                 std::vector<Sprite>& sprites){

    canvas.drawRectangle(0, 0, MAIN_WIDTH, MAIN_HEIGHT/2, packColor(40, 40, 40)); // Ceiling
    canvas.drawRectangle(0, MAIN_HEIGHT/2, MAIN_WIDTH, MAIN_HEIGHT/2, packColor(20, 20, 20)); // Floor
    
    const size_t textureCount = textures.getCount();
    const size_t textureSize = textures.getSize();

    // Cache closest distance between player and the block at x = i
    std::vector<float> distBuffer(MAIN_WIDTH, 1e3);

    for(size_t i = 0; i < MAIN_WIDTH; i++){
        float rotation = player.rot - player.fov / 2 + player.fov * (i/(float)MAIN_WIDTH);
        uint32_t color = packColor(255, 0, 0);
        
        // Cast single ray in certain direction
        for (float dist = 0; dist < 23; dist += 0.01){
            float targetX = player.x + dist * cos(rotation);
            float targetY = player.y + dist * sin(rotation);

            // Ray hits a block, render vertical column for 3D view
            if (!map.isEmptyAt((int) targetX, (int) targetY)){
                size_t textureIdx = map.getValueAt((int) targetX, (int) targetY);
                assert(textureIdx < textureCount);
                
                float distFixed = dist * cos(rotation - player.rot);
                distBuffer[i] = distFixed;
                size_t h = MAIN_HEIGHT / distFixed; // Fix fisheye distortion

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

    // Sort sprites by distance first and then render them
    assert(sortSpritesByDistance(sprites, player));
    for (size_t i = 0; i < sprites.size(); i++){
        assert(renderWorldSprite(canvas, sprites[i], spriteTextures, player, distBuffer));
    }
    return true;
}

bool renderMapPlayer(Canvas& canvas, Player& player){
    // Draw player on the top-down map
    canvas.drawRectangle(HUD_SHIFT_X + player.x * MAP_RECT_WIDTH,
                         HUD_SHIFT_Y + player.y * MAP_RECT_HEIGHT,5, 5, packColor(0, 255, 0));
    return true;
}

bool renderMapSprites(Canvas& canvas, std::vector<Sprite>& sprites){
    for (size_t i = 0; i < sprites.size(); i++){
        canvas.drawRectangle(HUD_SHIFT_X + sprites[i].x * MAP_RECT_WIDTH,
                             HUD_SHIFT_Y + sprites[i].y * MAP_RECT_HEIGHT, 5, 5, packColor(255, 0, 0));
    }
    return true;
}

bool renderMap(Canvas& canvas, Map& map, Texture& textures){
    // Scale the top-down map to window size and display it
    const size_t textureCount = textures.getCount();
    const size_t textureSize = textures.getSize();

    // Draw the top-down map
    for (size_t x = 0; x < MAP_WIDTH; x++){
        for (size_t y = 0; y < MAP_HEIGHT; y++){
            size_t imgX = HUD_SHIFT_X + x * MAP_RECT_WIDTH;
            size_t imgY = HUD_SHIFT_Y + y * MAP_RECT_HEIGHT;
            if (map.isEmptyAt(x, y)){
                canvas.drawRectangle(imgX, imgY, MAP_RECT_WIDTH, MAP_RECT_HEIGHT, packColor(0, 0, 0));
                continue;
            }

            size_t textureIdx = map.getValueAt(x, y);
            assert(textureIdx < textureCount);
            // Pick a pixel to represent the block
            canvas.drawRectangle(imgX, imgY, MAP_RECT_WIDTH, MAP_RECT_HEIGHT, textures.getValueAt(textureIdx, 0, 0));
        }
    }
    return true;
}

bool renderHUDPlaceholder(Canvas& canvas){
    canvas.drawRectangle(HUD_SHIFT_X, HUD_SHIFT_Y, HUD_WIDTH, HUD_HEIGHT, packColor(50, 50, 50));
    return true;
}

void render(Canvas& canvas,
            Map& map, 
            Texture& textures, 
            Texture& spriteTextures, 
            Player& player, 
            std::vector<Sprite>& sprites){
    canvas.clearCanvas(packColor(255, 255, 255));

    assert(renderWorld(canvas, map, textures, spriteTextures, player, sprites));

    if (HUD_DISPLAY){
        assert(renderHUDPlaceholder(canvas));
    }

    if (MAP_DISPLAY){
        assert(renderMap(canvas, map, textures));
        assert(renderMapPlayer(canvas, player));
        assert(renderMapSprites(canvas, sprites));
    }
}

#endif