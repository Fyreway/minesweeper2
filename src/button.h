#pragma once

#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>

struct Button {
    int x, y, w, h;
    int scale;
    const char *text;
};

struct Button create_button(SDL_Window *win,
                            SDL_Texture *tex,
                            int x,
                            int y,
                            int scale,
                            const char *text);

void update_button_pos(struct Button *btn, SDL_Window *win, int x_, int y_);

bool button_inside(struct Button btn, int mx, int my);

void render_button(SDL_Window *win,
                   SDL_Renderer *rend,
                   SDL_Texture *tex,
                   TTF_Font *font,
                   struct Button btn);
