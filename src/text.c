#include "text.h"

#include "util.h"

TTF_Font *load_font(int pt) {
    TTF_Font *font;
    try_sdl_nonnull(font = TTF_OpenFont("font/opensans.ttf", pt),
                    "Failed to load \"font/opensans.ttf\"");
    return font;
}

void render_text(SDL_Window *win,
                 SDL_Renderer *rend,
                 TTF_Font *font,
                 const char *text,
                 int x,
                 int y) {
    SDL_Surface *surf;
    SDL_Texture *tex;
    try_sdl_nonnull(
        surf = TTF_RenderText_Blended(
            font,
            text,
            (SDL_Color){.r = 255, .g = 255, .b = 255, .a = SDL_ALPHA_OPAQUE}),
        "Failed to render text");
    try_sdl_nonnull(tex = SDL_CreateTextureFromSurface(rend, surf),
                    "Failed to create text texture from surface");
    SDL_FreeSurface(surf);
    int w, h;
    int sw, sh;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    if (win == NULL && (x == -1 || y == -1)) return;
    int x_ = x, y_ = y;
    if (win != NULL) {
        SDL_GetWindowSize(win, &sw, &sh);
        x_ = x == -1 ? (sw - w) / 2 : x;
        y_ = y == -1 ? (sh - h) / 2 : y;
    }
    SDL_Rect rect = {x_, y_, w, h};
    SDL_RenderCopy(rend, tex, NULL, &rect);
    SDL_DestroyTexture(tex);
}
