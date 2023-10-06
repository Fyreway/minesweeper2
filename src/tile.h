#pragma once

#include <stdbool.h>
#include <stdint.h>

// Tile: 8 bit
// 0000 STATUS
// ---
// 0000 = NOT FLAGGED
// 0001 = FLAGGED
// 0100 = MINE AND NOT FLAGGED
// 0101 = FLAGGED AND MINE
// 0010 = MINED
// 0110 = MINE AND MINED
// 0111 = ILLEGAL
// ---
// 0000 VALUE

enum : uint8_t {
    T_FLAGGED = 0b0001 << 4,
    T_IS_MINE = 0b0100 << 4,
    T_MINED = 0b0010 << 4
};

typedef enum : uint8_t {
    M_FLAGGED,
    M_IS_MINE,
    M_MINED,
    M_OK,
    M_ZERO
} MineStatus;
typedef enum : uint8_t { F_MINED, F_UNFLAG, F_FLAG, F_NO_FLAGS } FlagStatus;

typedef uint8_t Tile;

// Interface
bool tile_is_flagged(Tile tile);
bool tile_is_mine(Tile tile);
bool tile_is_mined(Tile tile);
uint8_t tile_get_value(Tile tile);

MineStatus tile_mine(Tile tile);
FlagStatus tile_flag(Tile tile, uint8_t flags);
