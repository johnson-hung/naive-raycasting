#include <vector>
#include <string>
#include "map.h"
#include <iostream>

Map::Map(){
    std::string mapData = MAP;
    map = std::vector<char>();
    std::copy(mapData.begin(), mapData.end(), std::back_inserter(map));
    assert(map.size() == MAP_WIDTH*MAP_HEIGHT);
}

int Map::getValueAt(size_t x, size_t y){
    assert(x < MAP_WIDTH && y < MAP_HEIGHT && map.size() == MAP_WIDTH*MAP_HEIGHT);
    return map[y*MAP_WIDTH + x] - '0';
}

bool Map::isEmptyAt(size_t x, size_t y){
    assert(x < MAP_WIDTH && y < MAP_HEIGHT && map.size() == MAP_WIDTH*MAP_HEIGHT);
    return map[y*MAP_WIDTH + x] == ' ';
}