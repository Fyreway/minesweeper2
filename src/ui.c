#include "ui.h"

#include <stdio.h>

#include <SDL2_image/SDL_image.h>

#define try_sdl(cond, msg) \
    if (cond) f_sdl_err(msg)

#define try_sdl_nonnull(var, msg) try_sdl((var) == NULL, msg)

#define try_sdl_nonneg(var, msg) try_sdl((var) < 0, msg)

void init(struct SDLState *state) {
    try_sdl_nonneg(SDL_Init(SDL_INIT_VIDEO), "Failed to initialize SDL");
    try_sdl(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG,
            "Failed to initialize SDL_image with PNG");

    try_sdl_nonnull(state->win = SDL_CreateWindow(
                        "Minesweeper2",
                        SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED,
                        800,
                        600,
                        SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE),
                    "Failed to initialize SDL_Window");
    try_sdl_nonnull(state->rend = SDL_CreateRenderer(
                        state->win,
                        -1,
                        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
                    "Failed to initialize SDL_Renderer");
    try_sdl_nonnull(
        state->spritesheet = IMG_LoadTexture(state->rend, "spritesheet.png"),
        "Failed to load \"spritesheet.png\"");
}

void cleanup(struct SDLState state) {
    SDL_DestroyTexture(state.spritesheet);

    SDL_DestroyRenderer(state.rend);
    SDL_DestroyWindow(state.win);
    IMG_Quit();
    SDL_Quit();
}

void f_sdl_err(const char *msg) {
    fprintf(stderr, "minesweeper2: %s: %s\n", msg, SDL_GetError());
    exit(1);
}

void handle_events(bool *running, struct Map *map) {
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
            }
            break;
        default: break;
        }
    }
}

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

void render(struct SDLState state, struct Map map) {
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

    SDL_RenderPresent(state.rend);
}
