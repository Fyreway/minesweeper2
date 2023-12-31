#pragma once

#include <stdbool.h>
#include <stdint.h>

struct Tile {
    bool flagged : 1;
    bool is_mine : 1;
    bool mined : 1;
    uint8_t value : 4;
};

struct Tile create_mine();
struct Tile create_value(uint8_t value);

enum MineStatus : uint8_t {
    M_FLAGGED,
    M_IS_MINE,
    M_MINED,
    M_OK,
    M_ZERO
} tile_mine(struct Tile tile);
enum FlagStatus : uint8_t {
    F_MINED,
    F_UNFLAG,
    F_FLAG,
    F_NO_FLAGS
} tile_flag(struct Tile tile, uint8_t flags);
