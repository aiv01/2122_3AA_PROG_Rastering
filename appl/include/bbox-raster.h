#pragma once
#include "screen.h"
#include <stdbool.h>

typedef struct vector2_t {
    int x;
    int y;
} vector2_t;

int _min3(int v1, int v2, int v3);
int _max3(int v1, int v2, int v3);
// VET1x * VET2y - VET1y * VET2x
int _det(vector2_t* p, vector2_t* v1, vector2_t* v2);
bool bbox_is_point_in_triangle(vector2_t* P, vector2_t* A, vector2_t* B, vector2_t* C);
void bbox_triangle_raster(screen_t* screen, vector2_t* p1, vector2_t* p2, vector2_t* p3, color_t c);