#ifndef CANVAS_H
#define CANVAS_H

#include <cstdint>
#include <vector>

class Canvas{
    private:
        size_t w;
        size_t h;
        std::vector<uint32_t> img; // Current image

    public:
        Canvas(){};
        Canvas(const size_t width, const size_t height, const uint32_t color);

        // Clean up canvas with given color
        void clearCanvas(const uint32_t color);

        // Draw a pixel with given color at (x, y)
        void drawPixel(const size_t x, const size_t y, const uint32_t color);

        // Draw a rectangle starting from (x, y) and fill it with given color
        void drawRectangle(const size_t x, const size_t y, const size_t rectW, const size_t rectH, const uint32_t color);

        // Return current image
        std::vector<uint32_t> getImage(){ return img; }

        size_t getWidth(){ return w; }
        size_t getHeight(){ return h; }
};

#endif