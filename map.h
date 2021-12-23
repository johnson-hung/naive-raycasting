#ifndef MAP_H
#define MAP_H

#include <cstdlib>
#include <vector>

#define MAP_WIDTH 16
#define MAP_HEIGHT 16
#define MAP "0111111111111111"\
            "0              1"\
            "4              1"\
            "4              1"\
            "4     22211    1"\
            "1  0      1    1"\
            "1         1    0"\
            "0    1    1    0"\
            "0    3    1    1"\
            "1    1         1"\
            "1    1         5"\
            "1    11441     5"\
            "1              5"\
            "1            333"\
            "1              1"\
            "1111111111111111"

class Map{
    private:
        std::vector<char> map;

    public:
        Map();

        // Get data at (x, y) on the map
        int getValueAt(size_t x, size_t y);

        // Check if the cell at (x, y) is empty
        bool isEmptyAt(size_t x, size_t y);
};

#endif