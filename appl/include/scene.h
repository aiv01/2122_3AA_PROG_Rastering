#ifndef SCENE_H
#define SCENE_H
#include "SDL.h"
#include "screen.h"
typedef struct scene_t {
    screen_t* screen;
} scene;

scene* scene_create(int w, int h, SDL_Renderer*);
void scene_update(scene*, float delta_time);
void scene_destroy(scene*);

#endif //SCENE_H