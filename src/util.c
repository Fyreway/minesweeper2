#include "util.h"

#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

void f_sdl_err(const char *msg) {
    fprintf(stderr, "minesweeper2: %s: %s\n", msg, SDL_GetError());
    exit(1);
}
