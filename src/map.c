#include "map.h"

#include <stdlib.h>

struct Map create_map(uint8_t cols, uint8_t rows, uint8_t mines) {
    struct Tile **map = calloc(rows, sizeof(struct Tile *));
    for (uint8_t i = 0; i < rows; i++)
        map[i] = calloc(cols, sizeof(struct Tile));
    return (
        struct Map){map, cols, rows, mines, .flags = mines, .reveal = false};
}

void destroy_map(struct Map *map) {
    for (uint8_t i = 0; i < map->rows; i++)
        free(map->map[i]);
    free(map->map);
}

bool check_mine_pos(struct Map map,
                    int8_t x,
                    int8_t y,
                    uint8_t rx,
                    uint8_t ry) {
    if (x != -1 && y != -1
        && (!(rx > x + 1 || rx < x - 1) || !(ry > y + 1 || ry < y - 1)))
        return false;
    return map.map[ry][rx].is_mine;
}

void generate_mines(struct Map *map, int8_t x, int8_t y) {
    for (uint8_t i = 0; i < map->mines; i++) {
        uint8_t rx, ry;
        do {
            rx = rand() % map->cols;
            ry = rand() % map->rows;
        } while (!check_mine_pos(*map, x, y, rx, ry));
        map->map[ry][rx] = create_mine();
    }
}

#define adjacent(o, x, y, mx, my, f)                \
    if ((y) > 0) f((o)[(y)-1][(x)]);                \
    if ((y) < (my)-1) f((o)[(y) + 1][(x)]);         \
    if ((x) > 0) {                                  \
        f((o)[(y)][(x)-1]);                         \
        if ((y) > 0) f((o)[(y)-1][(x)-1]);          \
        if ((y) < (my)-1) f((o)[(y) + 1][(x)-1]);   \
    }                                               \
    if ((x) < (mx)-1) {                             \
        f((o)[(y)][(x) + 1]);                       \
        if ((y) > 0) f((o)[(y)-1][(x) + 1]);        \
        if ((y) < (my)-1) f((o)[(y) + 1][(x) + 1]); \
    }

void mark_tiles(struct Map *map) {
    uint8_t mines;
    for (uint8_t i = 0; i < map->rows; i++) {
        for (uint8_t j = 0; j < map->cols; j++) {
            if (map->map[i][j].is_mine) continue;
            mines = 0;
            adjacent(map->map, j, i, map->cols, map->rows, { mines++; });
            map->map[i][j].value = mines;
        }
    }
}

void generate_map(struct Map *map) {
    generate_mines(map, -1, -1);
    mark_tiles(map);
}
