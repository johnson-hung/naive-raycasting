#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint> // Fixed width integer types 
#include <cassert> // Error handling
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define PI 3.14159265
// #define _BG_GRADIENT_

// Turn (r, g, b, a) to 32-bit integer
uint32_t packColor(const uint8_t r,
                   const uint8_t g, 
                   const uint8_t b, 
                   const uint8_t a = 255){
    return (a << 24) + (b << 16) + (g << 8) + r;
}

// Turn 32-bit integer to (r, g, b, a)
void unpackColor(const uint32_t& color, 
                 uint8_t& r, 
                 uint8_t& g, 
                 uint8_t& b, 
                 uint8_t& a){
    r = color & 255;
    g = (color >> 8) & 255;
    b = (color >> 16) & 255;
    a = (color >> 24) & 255;
}

// Write PPM image data to given file
void generateImage(const std::string filename,
                   const std::vector<uint32_t>& image, 
                   const size_t w, 
                   const size_t h){
    assert(image.size() == w * h);

    std::ofstream ofs(filename, std::ios::binary); // Output stream to operate on file
    ofs << "P6\n" << w << " " << h << "\n255\n"; // Set image properties (P6: binary color image)

    // Set color for each cell in the grid
    for (size_t i = 0; i < w * h; i++){
        uint8_t r, g, b, a;
        unpackColor(image[i], r, g, b, a);
        ofs << static_cast<char>(r) << static_cast<char>(g) << static_cast<char>(b);
    }
    ofs.close();
}

void drawRectangle(std::vector<uint32_t>& img,
                   const size_t winW,
                   const size_t winH, 
                   const size_t x, // Start from this x
                   const size_t y, // Start from this y
                   const size_t rectW,
                   const size_t rectH,
                   const uint32_t color){
    assert(img.size() == winW * winH);

    for (size_t offsetX = 0; offsetX < rectW; offsetX++){
        for (size_t offsetY = 0; offsetY < rectH; offsetY++){
            size_t curX = x + offsetX;
            size_t curY = y + offsetY;
            if (curX >= winW || curY >= winH){ // No need to draw when it's out-of-bound
                continue;
            }
            img[curY * winW + curX] = color;
        }
    }   
}

bool loadTexture(const std::string filename,
                 std::vector<uint32_t>& texture,
                 size_t& size,
                 size_t& count){
    int numChannels = -1;
    int w;
    int h;
    unsigned char* img = stbi_load(filename.c_str(), &w, &h, &numChannels, 0);

    if (!img){
        std::cerr << "Error: Cannot load textures" << std::endl;
        return false;
    }
    if (numChannels != 4){
        std::cerr << "Error: Texture is not 32-bit image" << std::endl;
        stbi_image_free(img);
        return false;
    }

    count = w / h;
    size = w / count;
    std::cout << "Count of textures = " << count << "; size of texture = " << size << std::endl;
    if (w != h * int(count)){
        std::cerr << "Error: Textures are not packed horizontally" << std::endl;
        stbi_image_free(img);
        return false;
    }

    texture = std::vector<uint32_t>(w * h);
    for (int x = 0; x < w; x++){
        for (int y = 0; y < h; y++){
            uint8_t r = img[(y*w + x)*4];
            uint8_t g = img[(y*w + x)*4 + 1];
            uint8_t b = img[(y*w + x)*4 + 2];
            uint8_t a = img[(y*w + x)*4 + 3];
            texture[y*w + x] = packColor(r, g, b, a);
        }
    }
    stbi_image_free(img);
    return true;
}

// Return texture data at the specific column
std::vector<uint32_t> getTextureColumn(const std::vector<uint32_t>& texture,
                                       const size_t textureCount,
                                       const size_t textureSize,
                                       const size_t textureIdx,
                                       const size_t start,
                                       const size_t colH){
    const size_t w = textureSize * textureCount;
    const size_t h = textureSize;
    assert(texture.size() == w * h && start < textureSize && textureIdx < textureCount);

    std::vector<uint32_t> col(colH);
    for (size_t y = 0; y < colH; y++){   
        size_t tx = textureIdx * textureSize + start;
        size_t ty = textureSize * (y / (float) colH);
        col[y] = texture[ty * w + tx];
    }
    return col;
}

int main(){
    const size_t winW = 1024; // Window width (top-down map and 3D view)
    const size_t winH = 512; // Window height
    std::vector<uint32_t> img(winW * winH, packColor(255, 255, 255)); // Initialize the image

    // Map properties
    const size_t mapW = 16;
    const size_t mapH = 16;
    const char map[] = "0111111111111111"\
                       "0              1"\
                       "4              1"\
                       "4              1"\
                       "4     22211    1"\
                       "1  0      1    1"\
                       "1         1    0"\
                       "0    1    1    0"\
                       "0    3    1    1"\
                       "1    1         1"\
                       "1    1         5"\
                       "1    11441     5"\
                       "1              5"\
                       "1            333"\
                       "1              1"\
                       "1111111111111111";

    assert(sizeof(map) == mapW * mapH + 1);

    // Player properties
    float playerPosX = 7.5;
    float playerPosY = 7.5;
    float playerRot = 45 * PI / 180; // Rotation
    float playerFov = PI / 3; // Field of view

    std::vector<uint32_t> texture;
    size_t textureSize;
    size_t textureCount;
    if (!loadTexture("./textures/wallTextures.png", texture, textureSize, textureCount)){
        std::cerr << "Failed to load wall textures" << std::endl;
        return -1;
    }

    // Set background color
    for (size_t x = 0; x < winW; x++){
        for (size_t y = 0; y < winH; y++){
            #ifdef _BG_GRADIENT_
            // Gradient color
            uint8_t r = 255 * (row / (float) winW);
            uint8_t g = 255 * (col / (float) winW);
            uint8_t b = 255 * ((row + col) / (float) (winW + winW));
            #else
            // Simple color (white)
            uint8_t r = 255;
            uint8_t g = 255;
            uint8_t b = 255;
            #endif

            img[y * winW + x] = packColor(r, g, b);
        }
    }

    // Scale the top-down map to window size and display it
    const size_t rectW = (winW/2) / mapW;
    const size_t rectH = winH / mapH;

    // Draw the top-down map
    for (size_t x = 0; x < mapW; x++){
        for (size_t y = 0; y < mapH; y++){
            if (map[y * mapW + x] == ' ') continue;
            size_t imgX = x * rectW;
            size_t imgY = y * rectH;
            size_t textureIdx = map[y * mapW + x] - '0';
            assert(textureIdx < textureCount);
            drawRectangle(img, winW, winH, imgX, imgY, rectW, rectH, texture[textureIdx*textureSize]);
        }
    }

    // Draw player on the top-down map
    drawRectangle(img, winW, winH, playerPosX * rectW, playerPosY * rectH, 5, 5, packColor(0, 0, 255));
    
    // Cast field of view on the top-down map and 3D view
    for(size_t i = 0; i < winW/2; i++){
        float rotation = playerRot - playerFov / 2 + playerFov * (i/(float)(winW/2));
        uint32_t color = packColor(255, 0, 0);
        
        // Cast single ray in certain direction
        for (float dist = 0; dist < 23; dist += 0.01){
            float targetX = playerPosX + dist * cos(rotation);
            float targetY = playerPosY + dist * sin(rotation);
            int x = targetX * rectW;
            int y = targetY * rectH;

            // FOV part
            img[y*winW + x] = color;

            // Ray hits a block, render vertical column for 3D view
            if (map[(int) targetY * mapW + (int) targetX] != ' '){
                size_t h = winH / (dist * cos(rotation - playerRot)); // Fix fisheye distortion
                size_t textureIdx = map[(int)targetY * mapW + (int)targetX] - '0';
                assert(textureIdx < textureCount);
                
                // Get fractional part of targetX and targetY to determine the 
                // position of the hitpoint (top/bottom or left/right side of block)
                float hitX = targetX - floor(targetX + 0.5); // ~0 -> hit left/right side of block
                float hitY = targetY - floor(targetY + 0.5); // ~0 -> hit top/bottom side of block
                int start = std::abs(hitY) > std::abs(hitX)? hitY * textureSize : hitX * textureSize;
                if (start < 0) start += textureSize; // Make sure start position is positive
                assert(start >= 0 && start < (int) textureSize);

                // Write the specific column
                std::vector<uint32_t> col = getTextureColumn(texture, textureCount, textureSize,
                                                             textureIdx, start, h);
                x = winW/2 + i;
                for (size_t j = 0; j < h; j++){
                    y = winH/2 - h/2 + j;
                    if (x < 0 || y >= (int) winH) continue;
                    img[y*winW + x] = col[j];
                }
                break;
            }
        }
    }

    // Generate 24-bit color image (.ppm)
    generateImage("./img/output_7.ppm", img, winW, winH);
    return 0;
}
