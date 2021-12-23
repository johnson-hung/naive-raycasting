#ifndef MAP_H
#define MAP_H

#include <cstdlib>
#include <vector>

class Map{
    private:
        std::vector<char> map;

    public:
        Map();
        int getWidth();
        int getHeight();

        // Get data at (x, y) on the map
        int getValueAt(size_t x, size_t y);

        // Check if the cell at (x, y) is empty
        bool isEmptyAt(size_t x, size_t y);
};

#endif