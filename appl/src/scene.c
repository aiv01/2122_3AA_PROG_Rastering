#include "scene.h"
#include "SDL.h"
#include "screen.h"
#include "line-raster.h"
#include "bbox-raster.h"

scene* scene_create(int screen_width, int screen_height, SDL_Renderer* r) {
    scene* s = (scene*)malloc(sizeof(scene)); 
    s->screen = screen_new(screen_width, screen_height, r);
    return s;
}

void scene_update(scene* s, float delta_time) {
    
    dda_line_raster(s->screen, 150, 200, 200, 100, color_red());
    
    dda_line_raster(s->screen, 100, 100, 150, 200, color_yellow());
    
    dda_line_raster(s->screen, 100, 100, 200, 100, color_green());

    vector2_t p1 = { 350, 400 };
    vector2_t p2 = { 300, 300 };
    vector2_t p3 = { 400, 300 };

    bbox_triangle_raster(s->screen, &p1, &p2, &p3, color_red());

    screen_blit(s->screen);
}

void scene_destroy(scene* s) {
    screen_free(s->screen);
    free(s);
}