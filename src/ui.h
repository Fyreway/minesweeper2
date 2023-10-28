#pragma once

#include "button.h"
#include "map.h"

#define TILE_SIZE   24
#define SOURCE_SIZE 16

struct UIState {
    SDL_Window *win;
    SDL_Renderer *rend;

    SDL_Texture *spritesheet;
    SDL_Texture *button;
    TTF_Font *title_font;
    TTF_Font *mid_font;
    TTF_Font *normal_font;

    struct Button play_again_btn;
    struct Button menu_btn;
    struct Button size0_btn;
    struct Button size1_btn;
    struct Button size2_btn;

    bool show_map;
};

void init(struct UIState *state);
void cleanup(struct UIState state);

enum EventStatus : uint8_t { E_NONE, E_QUIT, E_FLAG, E_MINE };

void handle_events(struct UIState *state, bool *running, struct Map *map);
void render(struct UIState state, struct Map map);
