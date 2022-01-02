#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <cstdint>
#include <string>

// Turn (r, g, b, a) to 32-bit integer
uint32_t packColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

// Turn 32-bit integer to (r, g, b, a)
void unpackColor(const uint32_t& color, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a);

// Write PPM image data to given file
void generateImage(const std::string filename, const std::vector<uint32_t>& image, const size_t w, const size_t h);

#endif