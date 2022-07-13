#ifndef _FSPEC_UTIL_HPP
#define _FSPEC_UTIL_HPP

#include <cstdio>
#include <SDL2/SDL.h>

static inline int sdl_check(int error, const char *file, int line) {
    if (error < 0) {
        std::fprintf(stderr, "%s:%d: SDL Error: code %d: %s \n", file, line, error, SDL_GetError());
    }

    return error;
}

#define SDL_CHECK(e) sdl_check((e), __FILE__, __LINE__)

static inline void *sdl_check_null(void *ptr, const char *file, int line) {
    if (ptr == NULL) {
        std::fprintf(stderr, "%s:%d: SDL Error: returned null pointer: %s \n", file, line, SDL_GetError());
    }

    return ptr;
}

#define SDL_CHECK_NULL(p) sdl_check_null((p), __FILE__, __LINE__)
#define SDL_CHECK_NULL2(t,p) ((t *) SDL_CHECK_NULL(p))

#endif