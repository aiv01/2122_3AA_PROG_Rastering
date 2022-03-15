#include "scene.h"
#include "SDL.h"
#include "screen.h"
#include "line-raster.h"
#include "bbox-raster.h"
#include "camera.h"
#include "scanline-raster.h"
#include <SDL_image.h>
#include "vgpu.h"

texture_t* texture_load(const char* path) {
    //SDL IMG
    SDL_Surface* surface = IMG_Load(path);
    if (surface == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not load image from %s", path);
        return NULL;
    }

    texture_t* text = (texture_t*)malloc(sizeof(texture_t));
    text->width = surface->w;
    text->height = surface->h;
    text->pixel_size = surface->format->BytesPerPixel;
    int data_size = text->width * text->height * text->pixel_size;
    text->data = (unsigned char*)malloc(data_size);
    //Populate texture_t
    memcpy(text->data, surface->pixels, data_size);

    SDL_FreeSurface(surface);
    return text;
}

void texture_free(texture_t* text) { 
    free(text->data);
    free(text);
}


scene* scene_create(int screen_width, int screen_height, SDL_Renderer* r) {
    scene* s = (scene*)malloc(sizeof(scene)); 
    s->screen = screen_new(screen_width, screen_height, r);
    s->camera = camera_new(60.f, screen_width, screen_height);
    //s->camera->position = (vector3_t){4.f, 6.f, 0.f};
    s->camera->position = (vector3_t){0.f, 0.f, 0.f};
    s->quad = obj_parser_parse("bin\\appl\\resources\\quad.obj");
    s->suzanne = obj_parser_parse("bin\\appl\\resources\\suzanne.obj");
    s->suzanne_rotation = 0.f;
    s->quad_texture = texture_load("bin\\appl\\resources\\smile.png");
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
    s->suzanne_rotation += 10.f * delta_time;
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
    
    vgpu_t gpu;
    gpu.screen = s->screen;
    gpu.flags = VGPU_FLAG_COLOR;

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

        vector3_t cp1 = camera_world_to_camera_point(s->camera, &wv1);
        vector3_t cp2 = camera_world_to_camera_point(s->camera, &wv2);
        vector3_t cp3 = camera_world_to_camera_point(s->camera, &wv3);

        vertex_t vx1;
        vx1.screen_pos = &sv1;
        vx1.color = &red;
        vx1.z_pos = cp1.z;

        vertex_t vx2;
        vx2.screen_pos = &sv2;
        vx2.color = &green;
        vx2.z_pos = cp2.z;

        vertex_t vx3;
        vx3.screen_pos = &sv3;
        vx3.color = &yellow;
        vx3.z_pos = cp3.z;

        scanline_raster(&gpu, &vx1, &vx2, &vx3);
    }
}

void draw_quad_scanline_texture(scene* s) {
    obj_mesh_t* quad = s->quad;

    vgpu_t gpu;
    gpu.screen = s->screen;
    gpu.texture = s->quad_texture;
    gpu.flags = VGPU_FLAG_TEXTURE;

    for(int i=0; i < quad->triangles_count; ++i) {
        obj_triangle_t* t = &(quad->triangles[i]);

        vector3_t* v1 = (vector3_t*)&(t->v1.position);
        vector3_t* v2 = (vector3_t*)&(t->v2.position);
        vector3_t* v3 = (vector3_t*)&(t->v3.position);

        vector3_t wv1 = vector3_mult_scalar(v1, 2.f);
        vector3_t wv2 = vector3_mult_scalar(v2, 2.f);
        vector3_t wv3 = vector3_mult_scalar(v3, 2.f);
        wv1.z = -10.f;
        wv2.z = -10.f;
        wv3.z = -10.f;

        vector2_t sv1 = camera_world_to_screen_point(s->camera, &wv1);
        vector2_t sv2 = camera_world_to_screen_point(s->camera, &wv2);
        vector2_t sv3 = camera_world_to_screen_point(s->camera, &wv3);

        vector3_t cp1 = camera_world_to_camera_point(s->camera, &wv1);
        vector3_t cp2 = camera_world_to_camera_point(s->camera, &wv2);
        vector3_t cp3 = camera_world_to_camera_point(s->camera, &wv3);

        vertex_t vx1;
        vx1.screen_pos = &sv1;
        vx1.z_pos = cp1.z;
        vx1.text_pos = (vector2f_t*)&(t->v1.uv);

        vertex_t vx2;
        vx2.screen_pos = &sv2;
        vx2.z_pos = cp2.z;
        vx2.text_pos = (vector2f_t*)&(t->v2.uv);

        vertex_t vx3;
        vx3.screen_pos = &sv3;
        vx3.z_pos = cp3.z;
        vx3.text_pos = (vector2f_t*)&(t->v3.uv);

        scanline_raster(&gpu, &vx1, &vx2, &vx3);
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

    //draw_suzanne_scanline(s, false, delta_time);

    draw_quad_scanline_texture(s);

    screen_blit(s->screen);
}

void scene_destroy(scene* s) {
    screen_free(s->screen);
    camera_free(s->camera);
    obj_parser_free(s->quad);
    obj_parser_free(s->suzanne);
    texture_free(s->quad_texture);
    free(s);
}