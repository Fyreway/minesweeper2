#pragma once

#define try_sdl(cond, msg) \
    if (cond) f_sdl_err(msg)

#define try_sdl_nonnull(var, msg) try_sdl((var) == NULL, msg)

#define try_sdl_nonneg(var, msg) try_sdl((var) < 0, msg)

void f_sdl_err(const char *msg);
