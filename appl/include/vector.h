#pragma once
#include <stdio.h>

#define _USE_MATH_DEFINES
#include <math.h>

typedef struct vector2_t {
    int x;
    int y;
} vector2_t;

typedef struct vector2f_t {
    float x;
    float y;
} vector2f_t;

//TODO: Renaming with ending f
typedef struct vector3_t {
    float x;
    float y;
    float z;
} vector3_t;

static vector3_t vector3_sub(vector3_t* v1, vector3_t* v2) {
    vector3_t result;
    result.x = v1->x - v2->x;
    result.y = v1->y - v2->y;
    result.z = v1->z - v2->z;
    return result;
}

static vector3_t vector3_mult_scalar(vector3_t* v, float scalar) {
    vector3_t result;
    result.x = v->x * scalar;
    result.y = v->y * scalar;
    result.z = v->z * scalar;
    return result;
}

static void vector3_debug(vector3_t* v) {
    printf("v: (%f, %f, %f)\n", v->x, v->y, v->z);
}

static vector3_t vector3_rotate_y(vector3_t* v, float degrees) {
    //x2 = r−u = cosβ*x1 − sinβ*y1
    //y2 = t+s = sinβ*x1 + cosβ*y1
    float rads = degrees * M_PI / 180.f;
    vector3_t result;
    result.x = cosf(rads) * v->x - sinf(rads) * v->z;
    result.y = v->y;
    result.z = sinf(rads) * v->x + cosf(rads) * v->z;
    return result;
} 
