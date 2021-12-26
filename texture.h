#ifndef TEXTURE_H
#define TEXTURE_H

#include <cstdlib>
#include <cstdint>
#include <vector>

class Texture{
    private:
        std::vector<uint32_t> img;
        size_t imgW;
        size_t imgH;
        size_t size; // Size of texture
        size_t count; // Total count of textures in the image

    public:
        Texture(const std::string filename, const uint32_t format);

        // Get data at (x, y) in the texture with textureIdx
        uint32_t& getValueAt(const size_t textureIdx, const size_t x, const size_t y);

        // Return data at the specific column in texture with textureIdx
        std::vector<uint32_t> getTextureColumn(const size_t textureIdx, const size_t start, const size_t colH);

        size_t getSize(){ return size; }
        size_t getCount(){ return count; }
        bool isEmpty(){ return count == 0; }
};

#endif