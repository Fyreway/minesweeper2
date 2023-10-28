#include "ui.h"

uint32_t callback(uint32_t interval, void *param) {
    struct Map *map = (struct Map *)param;
    if (++map->seconds > 9999) {
        map->status = MS_LOSE;
        map->reveal = true;
    }
    return 1000;
}

void handle_events(struct UIState *state, bool *running, struct Map *map) {
    uint8_t x, y;
    SDL_Event e;
    bool flag = (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT]);
    if (state->show_map && map->status != MS_PLAY) SDL_RemoveTimer(map->timer);
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_QUIT: *running = 0; break;
        case SDL_KEYDOWN:
            switch (e.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE: *running = 0; break;
            case SDL_SCANCODE_X:
                for (uint8_t i = 0; i < map->rows; i++)
                    for (uint8_t j = 0; j < map->cols; j++)
                        if (!(map->map[i][j].mined || map->map[i][j].is_mine))
                            map->map[i][j].mined = true;
                break;
            default: break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (state->show_map) {
                if (map->status == MS_PLAY) {
                    x = (e.button.x - (800 - TILE_SIZE * map->cols) / 2)
                        / TILE_SIZE;
                    y = (e.button.y - (600 - TILE_SIZE * map->rows) / 2)
                        / TILE_SIZE;
                    if (e.button.button == SDL_BUTTON_RIGHT
                        || (e.button.button == SDL_BUTTON_LEFT && flag))
                        map_flag(map, x, y);
                    else if (e.button.button == SDL_BUTTON_LEFT) {
                        if (map->first_move) {
                            generate_map(map, x, y);
                            map->first_move = false;
                            map->timer =
                                SDL_AddTimer(1000, callback, (void *)map);
                        }
                        map_mine(map, x, y);
                    }
                } else {
                    if (button_inside(state->play_again_btn,
                                      e.button.x,
                                      e.button.y))
                        *map = create_map(map->cols, map->rows, map->mines);
                    else if (button_inside(state->menu_btn,
                                           e.button.x,
                                           e.button.y))
                        state->show_map = false;
                }
            } else {
                if (button_inside(state->size0_btn, e.button.x, e.button.y)) {
                    *map = create_map(8, 8, 10);
                    state->show_map = true;
                } else if (button_inside(state->size1_btn,
                                         e.button.x,
                                         e.button.y)) {
                    *map = create_map(30, 16, 99);
                    state->show_map = true;
                } else if (button_inside(state->size2_btn,
                                         e.button.x,
                                         e.button.y)) {
                    *map =
                        create_map(800 / TILE_SIZE, 600 / TILE_SIZE - 5, 255);
                    state->show_map = true;
                }
            }
            break;
        default: break;
        }
    }
}
