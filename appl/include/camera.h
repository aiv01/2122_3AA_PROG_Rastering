#pragma once
#include "vector.h"
#include <math.h>
#include <stdbool.h>

typedef struct camera_t {
    vector3_t position;
    float tan_fov;
    float aspect_ratio;
    int width;
    int height;
    float z_near;
    float z_far;
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
    cam->z_near = 0.1f;
    cam->z_far = 10.f;
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

static vector3_t camera_world_to_camera_point(camera_t* camera, vector3_t* world_point) {
    vector3_t camera_point = vector3_sub(world_point, &camera->position);
    return camera_point;
}

static bool triangle_is_facing_camera(vector3_t* cp1, vector3_t* cp2, vector3_t* cp3) {
    vector3_t v12 = vector3_sub(cp2, cp1);
    vector3_t v13 = vector3_sub(cp3, cp1);

    vector3_t face_normal = vector3_cross(&v12, &v13);

    vector3_t cp1_to_camera = vector3_mult_scalar(cp1, -1.f);

    float dot = vector3_dot(&cp1_to_camera, &face_normal); 
    if (dot >= 0) return true;
    return false;
}

static bool triangle_is_within_camera(camera_t* camera, vector2_t* sv1, float cz1, vector2_t* sv2, float cz2, vector2_t* sv3, float cz3) {
    if (sv1->x < 0 && sv2->x < 0 && sv3->x < 0) return false;
    if (sv1->y < 0 && sv2->y < 0 && sv3->y < 0) return false;
    if (sv1->x >= camera->width && sv2->x >= camera->width && sv3->x >= camera->width) return false;
    if (sv1->y >= camera->height && sv2->y >= camera->height && sv3->y >= camera->height) return false;
    if (-cz1 < camera->z_near && -cz2 < camera->z_near && -cz3 < camera->z_near) return false;
    if (-cz1 > camera->z_far && -cz2 > camera->z_far && -cz3 > camera->z_far) return false;
    return true;
} 