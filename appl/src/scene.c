#include "scene.h"
#include "SDL.h"
#include "screen.h"
#include "line-raster.h"

scene* scene_create(int screen_width, int screen_height, SDL_Renderer* r) {
    scene* s = (scene*)malloc(sizeof(scene)); 
    s->screen = screen_new(screen_width, screen_height, r);
    return s;
}

void scene_update(scene* s, float delta_time) {
    dda_line_raster(s->screen, 10, 10, 130, 130);
    dda_line_raster(s->screen, 100, 100, 230, 530);

    screen_blit(s->screen);
}

void scene_destroy(scene* s) {
    screen_free(s->screen);
    free(s);
}