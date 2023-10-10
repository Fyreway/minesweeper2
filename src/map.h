#pragma once

#include "tile.h"

struct Map {
    struct Tile **map;
    uint8_t cols, rows;
    uint8_t mines, flags;
    bool reveal;
};

struct Map create_map(uint8_t cols, uint8_t rows, uint8_t mines);
void destroy_map(struct Map *map);

void generate_map(struct Map *map);
