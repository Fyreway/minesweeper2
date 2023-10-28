#include "ui.h"

#include <SDL2_image/SDL_image.h>

#include "text.h"
#include "util.h"

void load_fonts(struct UIState *state) {
    state->title_font = load_font(50);
    state->mid_font = load_font(40);
    state->normal_font = load_font(25);
}

void load_textures(struct UIState *state) {
    try_sdl_nonnull(
        state->spritesheet = IMG_LoadTexture(state->rend, "spritesheet.png"),
        "Failed to load \"spritesheet.png\"");
    try_sdl_nonnull(state->button = IMG_LoadTexture(state->rend, "button.png"),
                    "Failed to load \"button.png\"");
}

void init(struct UIState *state) {
    try_sdl_nonneg(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER),
                   "Failed to initialize SDL");
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

    state->play_again_btn = create_button(state->win,
                                          state->button,
                                          state->normal_font,
                                          -1,
                                          250,
                                          3,
                                          "Play Again");
    state->menu_btn = create_button(state->win,
                                    state->button,
                                    state->normal_font,
                                    -1,
                                    350,
                                    3,
                                    "Menu");
    state->size0_btn = create_button(state->win,
                                     state->button,
                                     state->mid_font,
                                     -1,
                                     300,
                                     5,
                                     "Small");
    state->size1_btn = create_button(state->win,
                                     state->button,
                                     state->mid_font,
                                     -1,
                                     400,
                                     5,
                                     "Medium");
    state->size2_btn = create_button(state->win,
                                     state->button,
                                     state->mid_font,
                                     -1,
                                     500,
                                     5,
                                     "Large");

    state->show_map = false;
}

void cleanup(struct UIState state) {
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
