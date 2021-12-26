#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>
#include "canvas.h"

Canvas::Canvas(const size_t width, const size_t height, const uint32_t color){
    w = width;
    h = height;
    Canvas::clearCanvas(color);
}

// Clean up canvas with given color
void Canvas::clearCanvas(const uint32_t color){
    img = std::vector<uint32_t>(w*h, color);
}

// Draw a pixel with given color at (x, y)
void Canvas::drawPixel(const size_t x, const size_t y, const uint32_t color){
    assert(img.size() == w*h);
    if (x > w || y > h) return;
    
    img[y*w + x] = color;
}

// Draw a rectangle starting from (x, y) and fill it with given color
void Canvas::drawRectangle(
                   const size_t x, // Start from this x
                   const size_t y, // Start from this y
                   const size_t rectW,
                   const size_t rectH,
                   const uint32_t color){
    assert(img.size() == w*h);

    for (size_t offsetX = 0; offsetX < rectW; offsetX++){
        for (size_t offsetY = 0; offsetY < rectH; offsetY++){
            size_t curX = x + offsetX;
            size_t curY = y + offsetY;
            if (curX >= w || curY >= h){ // No need to draw when it's out-of-bound
                continue;
            }
            Canvas::drawPixel(curX, curY, color);
        }
    }   
}

