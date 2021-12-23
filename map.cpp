#include <vector>
#include <string>
#include "map.h"
#include <iostream>
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

Map::Map(){
    std::string mapData = MAP;
    map = std::vector<char>();
    std::copy(mapData.begin(), mapData.end(), std::back_inserter(map));
    assert(map.size() == MAP_WIDTH*MAP_HEIGHT);
}

int Map::getWidth(){ return MAP_WIDTH; }
int Map::getHeight(){ return MAP_HEIGHT; }

int Map::getValueAt(size_t x, size_t y){
    assert(x < MAP_WIDTH && y < MAP_HEIGHT && map.size() == MAP_WIDTH*MAP_HEIGHT);
    return map[y*MAP_WIDTH + x] - '0';
}

bool Map::isEmptyAt(size_t x, size_t y){
    assert(x < MAP_WIDTH && y < MAP_HEIGHT && map.size() == MAP_WIDTH*MAP_HEIGHT);
    return map[y*MAP_WIDTH + x] == ' ';
}