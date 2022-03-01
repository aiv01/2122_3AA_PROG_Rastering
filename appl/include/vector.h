#pragma once
#include <stdio.h>
typedef struct vector2_t {
    int x;
    int y;
} vector2_t;

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
