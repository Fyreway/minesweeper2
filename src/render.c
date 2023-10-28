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

void render_start_menu(struct UIState ui_state) {
    render_text(ui_state.win,
                ui_state.rend,
                ui_state.title_font,
                "MINESWEEPER",
                -1,
                75);
    render_button(ui_state.rend, ui_state.size0_btn);
    render_button(ui_state.rend, ui_state.size1_btn);
    render_button(ui_state.rend, ui_state.size2_btn);
}

void render_end_menu(struct UIState ui_state, enum MapStatus status) {
    int w, h;
    SDL_GetWindowSize(ui_state.win, &w, &h);
    SDL_Rect rect = {.x = (w - 400) / 2, .y = (h - 300) / 2, 400, 300};
    SDL_SetRenderDrawColor(ui_state.rend, 0, 0, 0, 127);
    SDL_SetRenderDrawBlendMode(ui_state.rend, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(ui_state.rend, &rect);
    SDL_SetRenderDrawColor(ui_state.rend, 0, 0, 0, SDL_ALPHA_OPAQUE);

    render_text(ui_state.win,
                ui_state.rend,
                ui_state.title_font,
                status == MS_WIN ? "You Win" : "You Lose",
                -1,
                (h - 250) / 2);
    render_button(ui_state.rend, ui_state.play_again_btn);
    render_button(ui_state.rend, ui_state.menu_btn);
}

void render(struct UIState ui_state, struct Map map) {
    // if (ui_state.show_map)
    //     SDL_SetWindowSize(ui_state.win,
    //                       map.cols * TILE_SIZE,
    //                       map.rows * TILE_SIZE);
    // else
    //     SDL_SetWindowSize(ui_state.win, 800, 600);
    try_sdl_nonneg(SDL_RenderClear(ui_state.rend), "Failed to clear renderer");

    if (ui_state.show_map) {
        for (uint8_t i = 0; i < map.rows; i++) {
            for (uint8_t j = 0; j < map.cols; j++) {
                uint16_t x, y;
                find_tex_pos(map, i, j, &x, &y);
                SDL_Rect src = {x, y, .w = SOURCE_SIZE, .h = SOURCE_SIZE};
                SDL_Rect dst = {
                    .x = j * TILE_SIZE + (800 - TILE_SIZE * map.cols) / 2,
                    .y = i * TILE_SIZE + (600 - TILE_SIZE * map.rows) / 2,
                    TILE_SIZE,
                    TILE_SIZE};

                try_sdl_nonneg(SDL_RenderCopy(ui_state.rend,
                                              ui_state.spritesheet,
                                              &src,
                                              &dst),
                               "Failed to copy texture");
            }
        }

        if (map.status != MS_PLAY) render_end_menu(ui_state, map.status);
    } else
        render_start_menu(ui_state);

    SDL_RenderPresent(ui_state.rend);
}
