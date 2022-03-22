#pragma once
//#include <SDL.h>
typedef struct color_t {
    int r;
    int g;
    int b;
    int a;
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

static color_t color_gray_light() {
    return (color_t){128, 128, 128, 255};
}

static color_t color_mult(color_t* color, float scalar) {
    color_t result;
    result.r = color->r * scalar;
    result.g = color->g * scalar;
    result.b = color->b * scalar;
    result.a = color->a * scalar;
    return result;
}

static color_t color_add(color_t* c1, color_t* c2) {
    color_t r;
    r.r = c1->r + c2->r;
    r.g = c1->g + c2->g;
    r.b = c1->b + c2->b;
    r.a = c1->a + c2->a;
    return r;
}

static color_t color_clamp(color_t* c1) {
    color_t r;
    r.r = (c1->r > 255) ? 255 : c1->r;
    r.g = (c1->g > 255) ? 255 : c1->g;
    r.b = (c1->b > 255) ? 255 : c1->b;
    r.a = (c1->a > 255) ? 255 : c1->a;
    return r;
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
void screen_clear_color(screen_t* screen, color_t* color);
