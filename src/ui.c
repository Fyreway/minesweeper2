#include "ui.h"

#include <SDL2_image/SDL_image.h>

#include "text.h"
#include "util.h"

void load_fonts(struct State *state) {
    state->title_font = load_font(50);
    state->normal_font = load_font(25);
}

void load_textures(struct State *state) {
    try_sdl_nonnull(
        state->spritesheet = IMG_LoadTexture(state->rend, "spritesheet.png"),
        "Failed to load \"spritesheet.png\"");
    try_sdl_nonnull(state->button = IMG_LoadTexture(state->rend, "button.png"),
                    "Failed to load \"button.png\"");
}

void init(struct State *state) {
    try_sdl_nonneg(SDL_Init(SDL_INIT_VIDEO), "Failed to initialize SDL");
    try_sdl(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG,
            "Failed to initialize SDL_image with PNG");
    try_sdl(TTF_Init() == -1, "Failed to initialize SDL_ttf");

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
    load_textures(state);
    load_fonts(state);

    state->play_again_btn =
        create_button(state->win, state->button, -1, -1, 3, "Play Again");
}

void cleanup(struct State state) {
    SDL_DestroyTexture(state.spritesheet);
    SDL_DestroyTexture(state.button);

    TTF_CloseFont(state.title_font);
    TTF_CloseFont(state.normal_font);

    SDL_DestroyRenderer(state.rend);
    SDL_DestroyWindow(state.win);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void handle_events(struct State state, bool *running, struct Map *map) {
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

void render(struct State state, struct Map map) {
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
