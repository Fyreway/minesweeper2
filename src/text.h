#pragma once

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>

TTF_Font *load_font(int pt);

void render_text(SDL_Window *win,
                 SDL_Renderer *rend,
                 TTF_Font *font,
                 const char *text,
                 int x,
                 int y);
