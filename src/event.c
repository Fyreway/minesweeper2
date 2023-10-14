#include "ui.h"

void handle_events(struct UIState state, bool *running, struct Map *map) {
    uint8_t x, y;
    SDL_Event e;
    bool flag = (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT]);
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
            if (map->status == MS_PLAY) {
                x = e.button.x / TILE_SIZE;
                y = e.button.y / TILE_SIZE;
                if (e.button.button == SDL_BUTTON_RIGHT
                    || (e.button.button == SDL_BUTTON_LEFT && flag))
                    map_flag(map, x, y);
                else if (e.button.button == SDL_BUTTON_LEFT) {
                    if (map->first_move) {
                        generate_map(map, x, y);
                        map->first_move = false;
                    }
                    map_mine(map, x, y);
                }
            } else {
                if (button_inside(state.play_again_btn, e.button.x, e.button.y))
                    *map = create_map(map->cols, map->rows, map->mines);
            }
            break;
        default: break;
        }
    }
}
