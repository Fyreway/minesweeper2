#include "tile.h"

struct Tile create_mine(void) {
    return (struct Tile){.flagged = false,
                         .is_mine = true,
                         .mined = false,
                         .value = 0};
}

struct Tile create_value(uint8_t value) {
    return (struct Tile){.flagged = false,
                         .is_mine = false,
                         .mined = false,
                         .value = value & 0b00001111};
}

MineStatus tile_mine(struct Tile tile) {
    if (tile.mined)
        return M_MINED;
    else if (tile.flagged)
        return M_FLAGGED;
    else if (tile.is_mine)
        return M_IS_MINE;
    else if (!tile.value)
        return M_ZERO;
    return M_OK;
}

FlagStatus tile_flag(struct Tile tile, uint8_t flags) {
    if (tile.mined)
        return F_MINED;
    else if (tile.flagged)
        return F_UNFLAG;
    else if (!flags)
        return F_NO_FLAGS;
    return F_FLAG;
}
