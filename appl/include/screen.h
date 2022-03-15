#pragma once
//#include <SDL.h>
typedef struct color_t {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} color_t;

static color_t color_red() {
    return (color_t){255, 0, 0, 255};
}

static color_t color_yellow() {
    return (color_t){255, 255, 0, 255};
}

static color_t color_green() {
    return (color_t){0, 255, 0, 255};
}

struct SDL_Renderer;
typedef struct SDL_Renderer SDL_Renderer;

struct SDL_Texture;
typedef struct SDL_Texture SDL_Texture;

/*
struct Uint8;
typedef struct Uint8 Uint8;
*/

typedef struct screen_t {
    int width;
    int height;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    unsigned char* color_buffer;
    size_t color_buffer_size;
    float* depth_buffer;
    size_t depth_buffer_size;
} screen_t;

screen_t* screen_new(int width, int height, SDL_Renderer* renderer);
void screen_free(screen_t* screen);
void screen_put_pixel(screen_t* screen, int x, int y, float z, color_t color);
void screen_blit(screen_t* screen);
void screen_clear(screen_t* screen);

