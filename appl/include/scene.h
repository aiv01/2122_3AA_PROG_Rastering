#ifndef SCENE_H
#define SCENE_H
#include "SDL.h"
#include "screen.h"
#include "camera.h"
#include "obj-parser.h"

typedef struct texture_t {
    int width;
    int height;
    int pixel_size;
    unsigned char* data;
} texture_t;

typedef struct scene_t {
    screen_t* screen;
    camera_t* camera;
    obj_mesh_t* suzanne;
    float suzanne_rotation;
    
    obj_mesh_t* quad;
    texture_t* quad_texture;

    obj_mesh_t* trup;
    float trup_rotation;
    texture_t* trup_texture;

    vector3_t point_light_position;
} scene;

scene* scene_create(int w, int h, SDL_Renderer*);
void scene_update(scene*, float delta_time);
void scene_destroy(scene*);

#endif //SCENE_H