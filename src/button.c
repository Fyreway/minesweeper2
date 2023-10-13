#include "button.h"

#include "text.h"

struct Button create_button(SDL_Window *win,
                            SDL_Texture *tex,
                            int x,
                            int y,
                            int scale,
                            const char *text) {
    int w, h;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    struct Button btn = {.w = w * scale, .h = h * scale, scale, text};
    update_button_pos(&btn, win, x, y);
    return btn;
}

void update_button_pos(struct Button *btn, SDL_Window *win, int x_, int y_) {
    int sw, sh;
    SDL_GetWindowSize(win, &sw, &sh);
    btn->x = x_ == -1 ? (sw - btn->w) / 2 : x_;
    btn->y = y_ == -1 ? (sh - btn->h) / 2 : y_;
}

bool button_inside(struct Button btn, int mx, int my) {
    return mx > btn.x && mx < (btn.x + btn.w) && my > btn.y
           && my < (btn.y + btn.h);
}

void render_button(SDL_Window *win,
                   SDL_Renderer *rend,
                   SDL_Texture *tex,
                   TTF_Font *font,
                   struct Button btn) {
    SDL_Rect rect = {.x = btn.x, .y = btn.y, .w = btn.w, .h = btn.h};
    SDL_RenderCopy(rend, tex, NULL, &rect);
    render_text(win, rend, font, btn.text, btn.x + 10, btn.y + 5);
}
