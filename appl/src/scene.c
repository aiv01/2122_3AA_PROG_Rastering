#include "scene.h"
#include "SDL.h"
#include "screen.h"
#include "line-raster.h"
#include "bbox-raster.h"
#include "camera.h"
#include "scanline-raster.h"

scene* scene_create(int screen_width, int screen_height, SDL_Renderer* r) {
    scene* s = (scene*)malloc(sizeof(scene)); 
    s->screen = screen_new(screen_width, screen_height, r);
    s->camera = camera_new(60.f, screen_width, screen_height);
    //s->camera->position = (vector3_t){4.f, 6.f, 0.f};
    s->camera->position = (vector3_t){0.f, 0.f, 0.f};
    s->quad = obj_parser_parse("bin\\appl\\resources\\quad.obj");
    s->suzanne = obj_parser_parse("bin\\appl\\resources\\suzanne.obj");
    s->suzanne_rotation = 0.f;

    if (!s->quad) { puts("ERROR reading quad.obj"); }
    return s;
}

void draw_quad(scene* s) {
    obj_mesh_t* quad = s->quad;
    //printf("triangle count: %d\n", quad->triangles_count);
    for(int i=0; i < quad->triangles_count; ++i) {
        vector3_t* v1 = (vector3_t*)&(quad->triangles[i].v1.position);
        vector3_t* v2 = (vector3_t*)&(quad->triangles[i].v2.position);
        vector3_t* v3 = (vector3_t*)&(quad->triangles[i].v3.position);

        vector3_t wv1 = vector3_mult_scalar(v1, 2.f);
        vector3_t wv2 = vector3_mult_scalar(v2, 2.f);
        vector3_t wv3 = vector3_mult_scalar(v3, 2.f);
        wv1.z = -10.f;
        wv2.z = -10.f;
        wv3.z = -10.f;

        vector2_t sv1 = camera_world_to_screen_point(s->camera, &wv1);
        vector2_t sv2 = camera_world_to_screen_point(s->camera, &wv2);
        vector2_t sv3 = camera_world_to_screen_point(s->camera, &wv3);

        bbox_triangle_raster(s->screen, &sv1, &sv2, &sv3, color_red());
    }
}

void draw_suzanne(scene* s, bool fillMode, float delta_time) {
    obj_mesh_t* obj = s->suzanne;
    float scale = 2.f;
    s->suzanne_rotation += 2.f * delta_time;
    vector3_t traslation = {0.f, 0.f, 5.f};
    for(int i=0; i < obj->triangles_count; ++i) {
        vector3_t* v1 = (vector3_t*)&(obj->triangles[i].v1.position);
        vector3_t* v2 = (vector3_t*)&(obj->triangles[i].v2.position);
        vector3_t* v3 = (vector3_t*)&(obj->triangles[i].v3.position);

        vector3_t wv1 = vector3_mult_scalar(v1, 2.f);
        vector3_t wv2 = vector3_mult_scalar(v2, 2.f);
        vector3_t wv3 = vector3_mult_scalar(v3, 2.f);

        wv1 = vector3_rotate_y(&wv1, s->suzanne_rotation);
        wv2 = vector3_rotate_y(&wv2, s->suzanne_rotation);
        wv3 = vector3_rotate_y(&wv3, s->suzanne_rotation);

        wv1 = vector3_sub(&wv1, &traslation);
        wv2 = vector3_sub(&wv2, &traslation);
        wv3 = vector3_sub(&wv3, &traslation);

        vector2_t sv1 = camera_world_to_screen_point(s->camera, &wv1);
        vector2_t sv2 = camera_world_to_screen_point(s->camera, &wv2);
        vector2_t sv3 = camera_world_to_screen_point(s->camera, &wv3);

        if (fillMode) {
            bbox_triangle_raster(s->screen, &sv1, &sv2, &sv3, color_red());
        } else {
            dda_line_raster(s->screen, sv1.x, sv1.y, sv2.x, sv2.y, color_red());
            dda_line_raster(s->screen, sv2.x, sv2.y, sv3.x, sv3.y, color_yellow());
            dda_line_raster(s->screen, sv3.x, sv3.y, sv1.x, sv1.y, color_green());
        }
    }
}


void draw_suzanne_scanline(scene* s, bool fillMode, float delta_time) {
    obj_mesh_t* obj = s->suzanne;
    float scale = 2.f;
    s->suzanne_rotation += 2.f * delta_time;
    vector3_t traslation = {0.f, 0.f, 5.f};
    
    color_t red = color_red();
    color_t green = color_green();
    color_t yellow = color_yellow();
    
    for(int i=0; i < obj->triangles_count; ++i) {
        vector3_t* v1 = (vector3_t*)&(obj->triangles[i].v1.position);
        vector3_t* v2 = (vector3_t*)&(obj->triangles[i].v2.position);
        vector3_t* v3 = (vector3_t*)&(obj->triangles[i].v3.position);

        vector3_t wv1 = vector3_mult_scalar(v1, 2.f);
        vector3_t wv2 = vector3_mult_scalar(v2, 2.f);
        vector3_t wv3 = vector3_mult_scalar(v3, 2.f);

        wv1 = vector3_rotate_y(&wv1, s->suzanne_rotation);
        wv2 = vector3_rotate_y(&wv2, s->suzanne_rotation);
        wv3 = vector3_rotate_y(&wv3, s->suzanne_rotation);

        wv1 = vector3_sub(&wv1, &traslation);
        wv2 = vector3_sub(&wv2, &traslation);
        wv3 = vector3_sub(&wv3, &traslation);

        vector2_t sv1 = camera_world_to_screen_point(s->camera, &wv1);
        vector2_t sv2 = camera_world_to_screen_point(s->camera, &wv2);
        vector2_t sv3 = camera_world_to_screen_point(s->camera, &wv3);

        vertex_t vx1;
        vx1.screen_pos = &sv1;
        vx1.color = &red;

        vertex_t vx2;
        vx2.screen_pos = &sv2;
        vx2.color = &green;

        vertex_t vx3;
        vx3.screen_pos = &sv3;
        vx3.color = &yellow;

        scanline_raster(s->screen, &vx1, &vx2, &vx3);
    }
}

void scene_update(scene* s, float delta_time) {
    screen_clear(s->screen);


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
    //bbox_triangle_raster(s->screen, &p1, &p2, &p3, color_red());
    //bbox_triangle_raster(s->screen, &p1, &p2, &p3, color_red());

    //draw_quad(s);

    //draw_suzanne(s, false, delta_time);

    draw_suzanne_scanline(s, false, delta_time);

    screen_blit(s->screen);
}

void scene_destroy(scene* s) {
    screen_free(s->screen);
    camera_free(s->camera);
    obj_parser_free(s->quad);
    obj_parser_free(s->suzanne);
    free(s);
}