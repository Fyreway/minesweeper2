#include "map.h"

#include <stdlib.h>

struct Map create_map(uint8_t cols, uint8_t rows, uint8_t mines) {
    struct Tile **map = calloc(rows, sizeof(struct Tile *));
    for (uint8_t i = 0; i < rows; i++)
        map[i] = calloc(cols, sizeof(struct Tile));
    return (struct Map){map,
                        cols,
                        rows,
                        mines,
                        .flags = mines,
                        .reveal = false,
                        .first_move = true,
                        .status = MS_PLAY,
                        .timer = 0,
                        .timing = false,
                        .seconds = 0};
}

void destroy_map(struct Map *map) {
    for (uint8_t i = 0; i < map->rows; i++)
        free(map->map[i]);
    free(map->map);
}

bool check_mine_pos(struct Map map,
                    uint8_t x,
                    uint8_t y,
                    uint8_t rx,
                    uint8_t ry) {
    if (map.first_move)
        return !(rx > x + 1 || rx < x - 1 || ry > y + 1 || ry < y - 1);
    return map.map[ry][rx].is_mine;
}

void generate_mines(struct Map *map, uint8_t x, uint8_t y) {
    for (uint8_t i = 0; i < map->mines; i++) {
        uint8_t rx, ry;
        do {
            rx = rand() % map->cols;
            ry = rand() % map->rows;
        } while (check_mine_pos(*map, x, y, rx, ry));
        map->map[ry][rx] = create_mine();
    }
}

//! THIS IS AN ABOMINATION
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
#define adjacent_xy(x, y, mx, my, f)       \
    if ((y) > 0) f(y - 1, x);              \
    if ((y) < (my)-1) f(y + 1, x);         \
    if ((x) > 0) {                         \
        f(y, x - 1);                       \
        if ((y) > 0) f(y - 1, x - 1);      \
        if ((y) < (my)-1) f(y + 1, x - 1); \
    }                                      \
    if ((x) < (mx)-1) {                    \
        f(y, x + 1);                       \
        if ((y) > 0) f(y - 1, x + 1);      \
        if ((y) < (my)-1) f(y + 1, x + 1); \
    }

#define adj_unchecked(o, x, y, f) \
    f((o)[y - 1][x - 1]);         \
    f((o)[y][x - 1]);             \
    f((o)[y + 1][x - 1]);         \
    f((o)[y - 1][x]);             \
    f((o)[y + 1][x]);             \
    f((o)[y - 1][x + 1]);         \
    f((o)[y][x + 1]);             \
    f((o)[y + 1][x + 1]);

#define adj_unchecked_xy(x, y, f) \
    f(y - 1, x - 1);              \
    f(y, x - 1);                  \
    f(y + 1, x - 1);              \
    f(y - 1, x);                  \
    f(y + 1, x);                  \
    f(y - 1, x + 1);              \
    f(y, x + 1);                  \
    f(y + 1, x + 1);

#define is_mine_f(t) \
    if (t.is_mine) mines++

void mark_tiles(struct Map *map) {
    uint8_t mines;
    for (uint8_t i = 0; i < map->rows; i++) {
        for (uint8_t j = 0; j < map->cols; j++) {
            if (map->map[i][j].is_mine) continue;
            mines = 0;
            adjacent(map->map, j, i, map->cols, map->rows, is_mine_f);
            map->map[i][j].value = mines;
        }
    }
}

#undef is_mine_f

void generate_map(struct Map *map, uint8_t x, uint8_t y) {
    generate_mines(map, x, y);
    mark_tiles(map);
}

void map_flag(struct Map *map, uint8_t x, uint8_t y) {
    if (x >= map->cols || y >= map->rows) return;
    switch (tile_flag(map->map[y][x], map->flags)) {
    case F_UNFLAG:
        (map->flags)++;
        map->map[y][x].flagged = false;
        break;
    case F_FLAG:
        (map->flags)--;
        map->map[y][x].flagged = true;
        break;
    default: break;
    }
}

#define mine_adj(y, x) map_mine_(map, x, y, c + 1, completed, false)

void map_mine_(struct Map *map,
               uint8_t x,
               uint8_t y,
               uint32_t c,
               struct Pair *completed,
               bool enable_chord);

#define flagged_f(t) \
    if (t.flagged) flags++

void chord(struct Map *map,
           uint8_t x,
           uint8_t y,
           uint32_t c,
           struct Pair *completed) {
    uint8_t flags = 0;
    adjacent(map->map, x, y, map->cols, map->rows, flagged_f);
    if (flags == map->map[y][x].value) {
        adj_unchecked_xy(x, y, mine_adj);
    }
}

#undef flagged_f

void map_mine_(struct Map *map,
               uint8_t x,
               uint8_t y,
               uint32_t c,
               struct Pair *completed,
               bool enable_chord) {
    if (x >= map->cols || y >= map->rows) return;
    for (uint32_t i = 0; i < c; i++) {
        if (completed[i].x == x && completed[i].y == y && x != 0 && y != 0) {
            return;
        }
    }
    switch (tile_mine(map->map[y][x])) {
    case M_IS_MINE:
        map->map[y][x].mined = true;
        map->status = MS_LOSE;
        map->reveal = true;
        break;
    case M_MINED:
        completed[c] = (struct Pair){x, y};
        if (enable_chord) chord(map, x, y, c + 1, completed);
        break;
    case M_OK: map->map[y][x].mined = true; break;
    case M_ZERO:
        map->map[y][x].mined = true;
        completed[c] = (struct Pair){x, y};
        adj_unchecked_xy(x, y, mine_adj);
        break;
    default: break;
    }

    for (uint8_t i = 0; i < map->rows; i++) {
        for (uint8_t j = 0; j < map->cols; j++) {
            if (!(map->map[i][j].is_mine || map->map[i][j].mined)) return;
        }
    }
    map->status = MS_WIN;
    map->reveal = true;
}

#undef mine_adj

void map_mine(struct Map *map, uint8_t x, uint8_t y) {
    struct Pair *completed =
        calloc(map->cols * map->rows - map->mines, sizeof(struct Pair));
    map_mine_(map, x, y, 0, completed, true);
    free(completed);
}
