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
    screen->depth_buffer_size = width * height * sizeof(float);
    screen->depth_buffer = (float*)malloc(screen->depth_buffer_size);
    return screen;
}

void screen_free(screen_t* screen) {
    free(screen->depth_buffer);
    free(screen->color_buffer);
    screen->height = 0;
    screen->width = 0;
    screen->renderer = NULL;
    SDL_DestroyTexture(screen->texture);
    screen->texture = NULL;
    free(screen);
    
}

void screen_put_pixel(screen_t* screen, int x, int y, float z, color_t color) {
    if (x < 0 || x >= screen->width) return;
    if (y < 0 || y >= screen->height) return;

    int depth_index = screen->width * y + x;
    float current_z = screen->depth_buffer[depth_index];
    if (current_z > z) return;
    screen->depth_buffer[depth_index] = z;

    int index = (screen->width * y + x) * 4;

    color_t back_color;
    back_color.r = screen->color_buffer[index + 0];
    back_color.g = screen->color_buffer[index + 1];
    back_color.b = screen->color_buffer[index + 2];
    back_color.a = screen->color_buffer[index + 3];

    //color_t* back_color = (color_t*)(screen->color_buffer + index);
    //Alpha Blending: front_alpha * front_color + (1 - front_alpha)*back_color

    float alpha = (float)color.a / 255.f; // 0 = fully trasparent
    back_color.r = alpha * (float)color.r + (1.f - alpha) * back_color.r;
    back_color.g = alpha * (float)color.g + (1.f - alpha) * back_color.g;
    back_color.b = alpha * (float)color.b + (1.f - alpha) * back_color.b;
    back_color.a = color.a + (float)back_color.a * (1.f - alpha);
    
    screen->color_buffer[index + 0] = (unsigned char)back_color.r;
    screen->color_buffer[index + 1] = (unsigned char)back_color.g;
    screen->color_buffer[index + 2] = (unsigned char)back_color.b;
    screen->color_buffer[index + 3] = (unsigned char)back_color.a;
}

void screen_blit(screen_t* screen) {
    SDL_UpdateTexture(screen->texture, NULL, screen->color_buffer, screen->width * 4);
    SDL_RenderCopy(screen->renderer, screen->texture, NULL, NULL);
}

void screen_clear(screen_t* screen) {
    memset(screen->color_buffer, 0, screen->color_buffer_size);
    memset(screen->depth_buffer, 0xff, screen->depth_buffer_size);
}

void screen_clear_color(screen_t* screen, color_t* color) {
    for(int i=0; i < screen->width * screen->height; ++i) {
        screen->color_buffer[i * 4 + 0] = color->r;
        screen->color_buffer[i * 4 + 1] = color->g;
        screen->color_buffer[i * 4 + 2] = color->b;
        screen->color_buffer[i * 4 + 3] = color->a;
    }
    memset(screen->depth_buffer, 0xff, screen->depth_buffer_size);
}