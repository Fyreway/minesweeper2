#pragma once

#include <SDL2/SDL.h>

#include "map.h"

#define TILE_SIZE   24
#define SOURCE_SIZE 16

struct SDLState {
    SDL_Window *win;
    SDL_Renderer *rend;

    SDL_Texture *spritesheet;
};

void init(struct SDLState *state);
void cleanup(struct SDLState state);

void f_sdl_err(const char *msg);

enum EventStatus : uint8_t {
    E_NONE,
    E_QUIT,
    E_FLAG_ON,
    E_FLAG_OFF,
    E_FLAG,
    E_MINE
};

uint8_t handle_events(bool flag,
                      enum EventStatus events[8],
                      uint8_t *x,
                      uint8_t *y);
void render(struct SDLState state, struct Map map);
