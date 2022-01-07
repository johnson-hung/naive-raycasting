#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <cassert>
#include "SDL.h"
#include "utils.h"
#include "texture.h"

// Load image file into SDL_Surface and store color data with `img`
Texture::Texture(const std::string filename, const uint32_t format){
    SDL_Surface* load = SDL_LoadBMP(filename.c_str());
    if (!load){
        std::cerr << "Failed to load BMP: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Surface* sdlImg = SDL_ConvertSurfaceFormat(load, format, 0);
    SDL_FreeSurface(load);
    if (!sdlImg){
        std::cerr << "Failed to convert surface: " << SDL_GetError() << std::endl;
        return;
    }

    if (sdlImg->w * 4 != sdlImg->pitch){
        SDL_FreeSurface(sdlImg);
        std::cerr << "Error: Not a 32-bit texture image" << std::endl;
        return;
    }

    imgW = sdlImg->w;
    imgH = sdlImg->h;
    count = imgW / imgH;
    size = imgW / count;
    std::cout << "# different textures = " << count << "; size of texture = " << size << std::endl;

    if (imgW != imgH * count){
        SDL_FreeSurface(sdlImg);
        std::cerr << "Error: Textures are not packed horizontally" << std::endl;
        return;
    }

    uint8_t* buf = reinterpret_cast<uint8_t*>(sdlImg->pixels);
    img = std::vector<uint32_t>(imgW * imgH);
    for (size_t x = 0; x < imgW; x++){
        for (size_t y = 0; y < imgH; y++){
            uint8_t r = buf[(y*imgW + x)*4];
            uint8_t g = buf[(y*imgW + x)*4 + 1];
            uint8_t b = buf[(y*imgW + x)*4 + 2];
            uint8_t a = buf[(y*imgW + x)*4 + 3];
            img[y*imgW + x] = packColor(r, g, b, a);
        }
    }
    SDL_FreeSurface(sdlImg);
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

// Getters
size_t Texture::getSize(){ return size; }
size_t Texture::getCount(){ return count; }

// Check if we can find textures in the given image
bool Texture::isEmpty(){ return count == 0; }