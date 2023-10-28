#pragma once

#include "tile.h"

struct Map {
    struct Tile **map;
    uint8_t cols, rows;
    uint8_t mines, flags;
    bool reveal, first_move;
    enum MapStatus { MS_PLAY, MS_WIN, MS_LOSE } status;
};

struct Pair {
    uint8_t x, y;
};

struct Map create_map(uint8_t cols, uint8_t rows, uint8_t mines);
void destroy_map(struct Map *map);

void generate_map(struct Map *map, uint8_t x, uint8_t y);

void map_flag(struct Map *map, uint8_t x, uint8_t y);
void map_mine(struct Map *map, uint8_t x, uint8_t y);
