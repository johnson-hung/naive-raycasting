#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <cassert>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "utils.h"
#include "texture.h"

Texture::Texture(const std::string filename){
    int numChannels = -1;
    int w;
    int h;
    unsigned char* buf = stbi_load(filename.c_str(), &w, &h, &numChannels, 0);

    if (!buf){
        std::cerr << "Error: Cannot load textures" << std::endl;
        return;
    }
    if (numChannels != 4){
        std::cerr << "Error: Texture is not 32-bit image" << std::endl;
        stbi_image_free(buf);
        return;
    }

    imgW = w;
    imgH = h;
    count = imgW / imgH;
    size = imgW / count;
    std::cout << "Count of textures = " << count << "; size of texture = " << size << std::endl;

    if (imgW != imgH * count){
        std::cerr << "Error: Textures are not packed horizontally" << std::endl;
        stbi_image_free(buf);
        return;
    }

    img = std::vector<uint32_t>(imgW * imgH);
    for (int x = 0; x < w; x++){
        for (int y = 0; y < h; y++){
            uint8_t r = buf[(y*w + x)*4];
            uint8_t g = buf[(y*w + x)*4 + 1];
            uint8_t b = buf[(y*w + x)*4 + 2];
            uint8_t a = buf[(y*w + x)*4 + 3];
            img[y*w + x] = packColor(r, g, b, a);
        }
    }
    stbi_image_free(buf);
}

// Get data at (x, y) in the texture with textureIdx
uint32_t& Texture::getValueAt(const size_t textureIdx, const size_t x, const size_t y){
    assert(x < size && y < size && textureIdx < count);
    return img[y*imgW + textureIdx*size + x];
}

// Return data at the specific column in texture with textureIdx
std::vector<uint32_t> Texture::getTextureColumn(const size_t textureIdx, const size_t start, const size_t colH){
    assert(start < size && textureIdx < count);

    std::vector<uint32_t> col(colH);
    for (size_t y = 0; y < colH; y++){
        col[y] = getValueAt(textureIdx, start, size * (y / (float) colH));
    }
    return col;
}