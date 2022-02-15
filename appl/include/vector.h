#pragma once

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
