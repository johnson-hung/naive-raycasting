#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint> // Fixed width integer types 
#include <cassert> // Error handling

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

// Write PPM image 
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

int main(){
    const size_t winW = 512; // Window width
    const size_t winH = 512; // Window height
    std::vector<uint32_t> img(winW * winH, 255); // Initialize the image to red

    // Set gradient color
    for (size_t row = 0; row < winW; row++){
        for (size_t col = 0; col < winW; col++){
            uint8_t r = 255 * (row / (float) winW);
            uint8_t g = 255 * (col / (float) winW);
            uint8_t b = 255 * ((row + col) / (float) (winW + winW));
            img[row * winW + col] = packColor(r, g, b);
        }
    }

    generateImage("./output.ppm", img, winW, winH); // Generate 24-bit color image (.ppm)

    return 0;
}