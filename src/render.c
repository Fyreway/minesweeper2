#include "ui.h"

#include "text.h"
#include "util.h"

void find_tex_pos(struct Map map,
                  uint8_t i,
                  uint8_t j,
                  uint16_t *x,
                  uint16_t *y) {
    if (map.status == MS_WIN) {
        if (map.map[i][j].is_mine) {
            *x = 64;
            *y = 16;
        } else {
            *x = 0;
            *y = 16;
        }
    } else if (map.map[i][j].flagged) {
        if (!map.map[i][j].is_mine && map.reveal) {
            *x = 48;
            *y = 16;
        } else {
            *x = 16;
            *y = 16;
        }
    } else if (map.map[i][j].is_mine && map.reveal) {
        *x = 32;
        *y = 16;
    } else if (map.map[i][j].mined) {
        *x = 16 * map.map[i][j].value;
        *y = 0;
    } else {
        *x = 0;
        *y = 16;
    }
}

void render(struct UIState state, struct Map map) {
    SDL_SetWindowSize(state.win, map.cols * TILE_SIZE, map.rows * TILE_SIZE);
    try_sdl_nonneg(SDL_RenderClear(state.rend), "Failed to clear renderer");

    for (uint8_t i = 0; i < map.rows; i++) {
        for (uint8_t j = 0; j < map.cols; j++) {
            uint16_t x, y;
            find_tex_pos(map, i, j, &x, &y);
            SDL_Rect src = {x, y, .w = SOURCE_SIZE, .h = SOURCE_SIZE};
            SDL_Rect dst = {.x = j * TILE_SIZE,
                            .y = i * TILE_SIZE,
                            TILE_SIZE,
                            TILE_SIZE};

            try_sdl_nonneg(
                SDL_RenderCopy(state.rend, state.spritesheet, &src, &dst),
                "Failed to copy texture");
        }
    }

    if (map.status == MS_WIN || map.status == MS_LOSE) {
        int w, h;
        SDL_GetWindowSize(state.win, &w, &h);
        SDL_Rect rect = {.x = (w - 400) / 2, .y = (h - 300) / 2, 400, 300};
        SDL_SetRenderDrawColor(state.rend, 0, 0, 0, 127);
        SDL_SetRenderDrawBlendMode(state.rend, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(state.rend, &rect);
        SDL_SetRenderDrawColor(state.rend, 0, 0, 0, SDL_ALPHA_OPAQUE);

        render_text(state.win,
                    state.rend,
                    state.title_font,
                    map.status == MS_WIN ? "You Win" : "You Lose",
                    -1,
                    (h - 250) / 2);
        render_button(state.win,
                      state.rend,
                      state.button,
                      state.normal_font,
                      state.play_again_btn);
    }

    SDL_RenderPresent(state.rend);
}
