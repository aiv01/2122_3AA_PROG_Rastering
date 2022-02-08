#pragma once
#include <SDL.h>
typedef struct color_t {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
} color_t;

static color_t color_red() {
    return (color_t){255, 0, 0, 255};
}


typedef struct screen_t {
    int width;
    int height;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    Uint8* color_buffer;
} screen_t;

screen_t* screen_new(int width, int height, SDL_Renderer* renderer);
void screen_free(screen_t* screen);
void screen_put_pixel(screen_t* screen, int x, int y, color_t color);
void screen_blit(screen_t* screen);

