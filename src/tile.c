#include "tile.h"

bool tile_is_flagged(Tile tile) {
    return (tile & 0b00010000) >> 4;
}

bool tile_is_mined(Tile tile) {
    return (tile & 0b00100000) >> (4 + 1);
}

bool tile_is_mine(Tile tile) {
    return (tile & 0b01000000) >> (4 + 2);
}

uint8_t tile_get_value(Tile tile) {
    return tile & 0b00001111;
}

MineStatus tile_mine(Tile tile) {
    if (tile_is_mined(tile))
        return M_MINED;
    else if (tile_is_flagged(tile))
        return M_FLAGGED;
    else if (tile_is_mine(tile))
        return M_IS_MINE;
    else if (!tile_get_value(tile))
        return M_ZERO;
    return M_OK;
}

FlagStatus tile_flag(Tile tile, uint8_t flags) {
    if (tile_is_mined(tile))
        return F_MINED;
    else if (tile_is_flagged(tile))
        return F_UNFLAG;
    else if (flags == 0)
        return F_NO_FLAGS;
    return F_FLAG;
}
