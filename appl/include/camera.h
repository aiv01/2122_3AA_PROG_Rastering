#pragma once
#include "vector.h"
#include <math.h>

typedef struct camera_t {
    vector3_t position;
    float tan_fov;
    float aspect_ratio;
    int width;
    int height;
} camera_t;

static camera_t* camera_new(float vertical_fov_degrees, int width, int height) {
    camera_t* cam = (camera_t*)malloc(sizeof(camera_t));
    cam->position = (vector3_t){0, 0, 0};

    //vertical fov
    float fov_degrees = vertical_fov_degrees;
    float half_fov_degrees = fov_degrees * 0.5f;
    float half_fov_radians = half_fov_degrees * M_PI / 180.f;
    cam->tan_fov = tanf(half_fov_radians);
    cam->aspect_ratio = (float)width / (float)height;
    cam->width = width;
    cam->height = height;
    return cam;
}

static void camera_free(camera_t* camera) {
    free(camera);
}

static vector2_t camera_world_to_screen_point(camera_t* camera, vector3_t* world_point) {
    vector3_t camera_point = vector3_sub(world_point, &camera->position);

    //Range {-1, 1}
    float plane_x = camera_point.x / -camera_point.z; 
    float plane_y = camera_point.y / -camera_point.z; 

    //fix by fov
    plane_x /= camera->tan_fov;
    plane_y /= camera->tan_fov;

    //fix by aspect
    plane_x /= camera->aspect_ratio;

    int screen_x = (int) ( (plane_x + 1.f) * 0.5f * camera->width ); 
    int screen_y = (int) ( (1.f - (plane_y + 1.f) * 0.5f) * camera->height);

    return (vector2_t){screen_x, screen_y};
}