#include "scene.h"
#include "SDL.h"
#include "screen.h"
#include "line-raster.h"
#include "bbox-raster.h"
#include "camera.h"

scene* scene_create(int screen_width, int screen_height, SDL_Renderer* r) {
    scene* s = (scene*)malloc(sizeof(scene)); 
    s->screen = screen_new(screen_width, screen_height, r);
    s->camera = camera_new(60.f, screen_width, screen_height);
    s->camera->position = (vector3_t){4.f, 6.f, 0.f};
    return s;
}

void scene_update(scene* s, float delta_time) {
    
    dda_line_raster(s->screen, 150, 200, 200, 100, color_red());
    
    dda_line_raster(s->screen, 100, 100, 150, 200, color_yellow());
    
    dda_line_raster(s->screen, 100, 100, 200, 100, color_green());


    //Triangolo: Coordinate World
    vector3_t tw1 = { 0.f, -4.f, -20};   
    vector3_t tw2 = { -4.f, 4.f, -20};
    vector3_t tw3 = {  4.f, 4.f, -20};

    //Triangolo: World_to_screen_point()
    
    /*
    vector2_t p1 = { 350, 400 }; //v
    vector2_t p2 = { 300, 300 };
    vector2_t p3 = { 400, 300 };
    */
    vector2_t p1 = camera_world_to_screen_point(s->camera, &tw1);
    vector2_t p2 = camera_world_to_screen_point(s->camera, &tw2);
    vector2_t p3 = camera_world_to_screen_point(s->camera, &tw3);
    bbox_triangle_raster(s->screen, &p1, &p2, &p3, color_red());

    bbox_triangle_raster(s->screen, &p1, &p2, &p3, color_red());

    screen_blit(s->screen);
}

void scene_destroy(scene* s) {
    screen_free(s->screen);
    camera_free(s->camera);
    free(s);
}