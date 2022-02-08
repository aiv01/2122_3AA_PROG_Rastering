#pragma once
#include <stdlib.h>
#include <SDL.h>

typedef struct screen_t {
    int width;
    int height;
    SDL_Renderer* renderer;
} screen_t;

static screen_t* screen_new(int width, int height, SDL_Renderer* renderer) {
    screen_t* screen = (screen_t*)malloc(sizeof(screen_t));
    screen->width = width;
    screen->height = height;
    screen->renderer = renderer;
    return screen;
}

 static void screen_free(screen_t* screen) {
     free(screen);
 }

static void screen_put_pixel(screen_t* screen, int x, int y) {
    SDL_SetRenderDrawColor(screen->renderer, 255, 0, 0, 255);
    SDL_RenderDrawPoint(screen->renderer, x, y);
}