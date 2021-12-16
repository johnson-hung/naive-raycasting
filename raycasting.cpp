#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint> // Fixed width integer types 
#include <cassert> // Error handling
#include <cmath>

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

int main(){
    const size_t winW = 1024; // Window width (top-down map and 3D view)
    const size_t winH = 512; // Window height
    std::vector<uint32_t> img(winW * winH, packColor(255, 255, 255)); // Initialize the image

    // Map properties
    const size_t mapW = 16;
    const size_t mapH = 16;
    const char map[] = "0000000000000000"\
                       "0              0"\
                       "0              0"\
                       "0              0"\
                       "0    000000    0"\
                       "0         0    0"\
                       "0         0    0"\
                       "0    0    0    0"\
                       "0    0    0    0"\
                       "0    0         0"\
                       "0    0         0"\
                       "0    000000    0"\
                       "0              0"\
                       "0              0"\
                       "0              0"\
                       "0000000000000000";

    assert(sizeof(map) == mapW * mapH + 1);

    // Player properties
    float playerPosX = 7.5;
    float playerPosY = 7.5;
    float playerRot = 45 * PI / 180; // Rotation
    float playerFov = PI / 3; // Field of view

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
    for (size_t x = 0; x < mapW; x++){
        for (size_t y = 0; y < mapH; y++){
            if (map[y * mapW + x] == ' ') continue;
            size_t imgX = x * rectW;
            size_t imgY = y * rectH;
            uint32_t color = packColor(0, 0, 0);
            drawRectangle(img, winW, winH, imgX, imgY, rectW, rectH, color);
        }
    }

    // Draw player on the top-down map
    drawRectangle(img, winW, winH, playerPosX * rectW, playerPosY * rectH, 5, 5, packColor(0, 0, 255));

    // Cast field of view on the top-down map and 3D view
    for(size_t i = 0; i < winW/2; i++){
        float rotation = playerRot - playerFov / 2 + playerFov * (i/(float)(winW/2));
        uint32_t color = packColor(255, 0, 0);
        
        // Cast single ray in certain direction
        for (float dist = 0; dist < 23; dist += 0.05){
            float targetX = playerPosX + dist * cos(rotation);
            float targetY = playerPosY + dist * sin(rotation);
            if (map[(int) targetY * mapW + (int) targetX] != ' '){
                // Ray hits a block, render vertical column for 3D view
                size_t h = winH / dist;
                uint32_t wallColor = packColor(255 * h / winH, 0, 0); // Some color variation
                drawRectangle(img, winW, winH, winW/2 + i, winH/2 - h/2, 1, h, wallColor);
                break;
            }
            size_t x = targetX * rectW;
            size_t y = targetY * rectH;
            img[y * winW + x] = color;
        }
    }

    // Generate 24-bit color image (.ppm)
    generateImage("./img/output_4.ppm", img, winW, winH);

    return 0;
}
