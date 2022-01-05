#ifndef TEXTURE_H
#define TEXTURE_H

#include <cstdlib>
#include <cstdint>
#include <vector>

class Texture{
    private:
        std::vector<uint32_t> img;  // Texture image file
        size_t imgW;                // Width of the image
        size_t imgH;                // Height of the image
        size_t size;                // Side length of a texture (assume it's a square)
        size_t count;               // Total count of textures in the image

    public:
        Texture(){};

        // Load image file into SDL_Surface and store color data with `img`
        Texture(const std::string filename, const uint32_t format);

        // Get data at (x, y) in the texture with textureIdx
        uint32_t& getValueAt(const size_t textureIdx, const size_t x, const size_t y);

        // Return data at the specific column in the texture with textureIdx
        std::vector<uint32_t> getTextureColumn(const size_t textureIdx, const size_t start, const size_t colH);

        // Getters
        size_t getSize();
        size_t getCount();

        // Check if we can find textures in the given image
        bool isEmpty();
};

#endif