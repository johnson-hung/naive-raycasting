#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint> // Fixed width integer types 
#include <cassert> // Error handling
#include <cmath>
#include "utils.h"
#include "canvas.h"
#include "map.h"
#include "texture.h"
#include "player.h"


#define PI              3.14159265

#define CANVAS_WIDTH    1024
#define CANVAS_HEIGHT   512
#define RECT_WIDTH      ((CANVAS_WIDTH/2) / MAP_WIDTH)
#define RECT_HEIGHT     (CANVAS_HEIGHT / MAP_HEIGHT)


bool renderMap(Canvas& canvas, Map& map, Texture& textures){
    // Scale the top-down map to window size and display it
    const size_t textureCount = textures.getCount();
    const size_t textureSize = textures.getSize();

    // Draw the top-down map
    for (size_t x = 0; x < MAP_WIDTH; x++){
        for (size_t y = 0; y < MAP_HEIGHT; y++){
            if (map.isEmptyAt(x, y)) continue;
            size_t imgX = x * RECT_WIDTH;
            size_t imgY = y * RECT_HEIGHT;
            size_t textureIdx = map.getValueAt(x, y);
            assert(textureIdx < textureCount);
            // Pick a pixel to represent the block
            canvas.drawRectangle(imgX, imgY, RECT_WIDTH, RECT_HEIGHT, textures.getValueAt(textureIdx, 0, 0));
        }
    }
    return true;
}

// Cast field of view on the top-down map and 3D view
bool renderWorld(Canvas& canvas, Map& map, Texture& textures, Player& player){
    const size_t textureCount = textures.getCount();
    const size_t textureSize = textures.getSize();

    for(size_t i = 0; i < CANVAS_WIDTH/2; i++){
        float rotation = player.rot - player.fov / 2 + player.fov * (i/(float)(CANVAS_WIDTH/2));
        uint32_t color = packColor(255, 0, 0);
        
        // Cast single ray in certain direction
        for (float dist = 0; dist < 23; dist += 0.01){
            float targetX = player.x + dist * cos(rotation);
            float targetY = player.y + dist * sin(rotation);
            int x = targetX * RECT_WIDTH;
            int y = targetY * RECT_HEIGHT;

            // Display FOV on the map if map is already rendered
            canvas.drawPixel(x, y, color);

            // Ray hits a block, render vertical column for 3D view
            if (!map.isEmptyAt((int) targetX, (int) targetY)){
                size_t h = CANVAS_HEIGHT / (dist * cos(rotation - player.rot)); // Fix fisheye distortion
                size_t textureIdx = map.getValueAt((int) targetX, (int) targetY);
                assert(textureIdx < textureCount);
                
                // Get fractional part of targetX and targetY to determine the 
                // position of the hitpoint (top/bottom or left/right side of block)
                float hitX = targetX - floor(targetX + 0.5); // ~0 -> hit left/right side of block
                float hitY = targetY - floor(targetY + 0.5); // ~0 -> hit top/bottom side of block
                int start = std::abs(hitY) > std::abs(hitX)? hitY * textureSize : hitX * textureSize;
                if (start < 0) start += textureSize; // Make sure start position is positive
                assert(start >= 0 && start < (int) textureSize);

                // Write the specific column
                std::vector<uint32_t> col = textures.getTextureColumn(textureIdx, start, h);
                x = CANVAS_WIDTH/2 + i;
                for (size_t j = 0; j < h; j++){
                    y = CANVAS_HEIGHT/2 - h/2 + j;
                    if (x < 0 || y >= (int) CANVAS_HEIGHT) continue;
                    canvas.drawPixel(x, y, col[j]);
                }
                break;
            }
        }
    }
    return true;
}

bool renderPlayer(Canvas& canvas, Player& player){
    // Draw player on the top-down map
    canvas.drawRectangle(player.x * RECT_WIDTH, player.y * RECT_HEIGHT, 5, 5, packColor(0, 0, 255));
    return true;
}

void render(Canvas& canvas, Map& map, Texture& textures, Player& player){
    canvas.clearCanvas(packColor(255, 255, 255));

    assert(renderWorld(canvas, map, textures, player));
    assert(renderMap(canvas, map, textures));
    assert(renderPlayer(canvas, player));
}


int main(){
    // Window (canvas) properties
    Canvas canvas(CANVAS_WIDTH, CANVAS_HEIGHT, packColor(255, 255, 255)); // Initialize the canvas

    // Initialize the map and get properties
    Map map;

    // Initialize the player
    Player player(7.5, 7.5, 45 * PI / 180, PI / 3);
    
    // Initialize the wall textures
    Texture wallTextures("./textures/wallTextures.png");
    if (wallTextures.isEmpty()){
        std::cerr << "Failed to load wall textures" << std::endl;
        return -1;
    }

    render(canvas, map, wallTextures, player);

    // Generate 24-bit color image (.ppm)
    generateImage("./img/output_8.ppm", canvas.getImage(), CANVAS_WIDTH, CANVAS_HEIGHT);
    return 0;
}
