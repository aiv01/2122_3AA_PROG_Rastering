#include "bbox-raster.h"

int _min3(int v1, int v2, int v3) {
    int result = v1;
    if (v2 < result) result = v2;
    if (v3 < result) result = v3;
    return result;
}

int _max3(int v1, int v2, int v3) {
    int result = v1;
    if (v2 > result) result = v2;
    if (v3 > result) result = v3;
    return result;
}
// VET1x * VET2y - VET1y * VET2x
int _det(vector2_t* p, vector2_t* v1, vector2_t* v2) {
    return (v2->x - v1->x) * (p->y - v1->y) - (v2->y - v1->y) * (p->x - v1->x);
}

bool bbox_is_point_in_triangle(vector2_t* P, vector2_t* A, vector2_t* B, vector2_t* C) {
    bool b1, b2, b3;
    b1 = _det(P, A, B) <= 0;
    b2 = _det(P, B, C) <= 0;
    b3 = _det(P, C, A) <= 0;
    return (b1 == b2) && (b2 == b3);
}

void bbox_triangle_raster(screen_t* screen, vector2_t* p1, vector2_t* p2, vector2_t* p3, color_t c) {
    //STEP1: Rectangle
    int x_min = _min3(p1->x, p2->x, p3->x);
    int y_min = _min3(p1->y, p2->y, p3->y);

    int x_max = _max3(p1->x, p2->x, p3->x);
    int y_max = _max3(p1->y, p2->y, p3->y);

    for(int x = x_min; x < x_max; ++x) {
        for(int y = y_min; y < y_max; ++y) {
            vector2_t point = {x, y};
            if (bbox_is_point_in_triangle(&point, p1, p2, p3)) {
                screen_put_pixel(screen, point.x, point.y, c);
            }
        }
    }
}