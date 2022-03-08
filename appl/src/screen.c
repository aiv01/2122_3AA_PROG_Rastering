#include "screen.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <string.h>

screen_t* screen_new(int width, int height, SDL_Renderer* renderer) {
    screen_t* screen = (screen_t*)malloc(sizeof(screen_t));
    screen->width = width;
    screen->height = height;
    screen->renderer = renderer;
    screen->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, width, height);
    screen->color_buffer_size = width * height * 4 * sizeof(Uint8);
    screen->color_buffer = (Uint8*)malloc(screen->color_buffer_size);
    return screen;
}

void screen_free(screen_t* screen) {
    free(screen->color_buffer);
    screen->height = 0;
    screen->width = 0;
    screen->renderer = NULL;
    SDL_DestroyTexture(screen->texture);
    screen->texture = NULL;
    free(screen);
    
}

void screen_put_pixel(screen_t* screen, int x, int y, color_t color) {
    if (x < 0 || x >= screen->width) return;
    if (y < 0 || y >= screen->height) return;

    int index = (screen->width * y + x) * 4;
    screen->color_buffer[index + 0] = color.r;
    screen->color_buffer[index + 1] = color.g;
    screen->color_buffer[index + 2] = color.b;
    screen->color_buffer[index + 3] = color.a;
}

void screen_blit(screen_t* screen) {
    SDL_UpdateTexture(screen->texture, NULL, screen->color_buffer, screen->width * 4);
    SDL_RenderCopy(screen->renderer, screen->texture, NULL, NULL);
}

void screen_clear(screen_t* screen) {
    memset(screen->color_buffer, 0, screen->color_buffer_size);
}