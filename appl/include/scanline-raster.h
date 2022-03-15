#pragma once
#include "vector.h"
#include "screen.h"
#include "vgpu.h"

typedef struct vertex_t {
    vector2_t* screen_pos; 
    color_t* color;
    float z_pos;
    vector2f_t* text_pos;
} vertex_t;



static void _sort_by_y(vertex_t** vx1, vertex_t** vx2, vertex_t** vx3) {
    //issue: We alwasy need the "most recent" screen_pos because is potentially swapped by vertex
    vertex_t* temp;
    if ((*vx1)->screen_pos->y > (*vx2)->screen_pos->y) { temp = *vx1; *vx1 = *vx2; *vx2 = temp; }
    if ((*vx2)->screen_pos->y > (*vx3)->screen_pos->y) { temp = *vx2; *vx2 = *vx3; *vx3 = temp; }
    if ((*vx1)->screen_pos->y > (*vx2)->screen_pos->y) { temp = *vx1; *vx1 = *vx2; *vx2 = temp; }
    
    /*
    vector2_t* p1 = (*vx1)->screen_pos;
    vector2_t* p2 = (*vx2)->screen_pos;
    vector2_t* p3 = (*vx3)->screen_pos;


    vertex_t* temp;

    if (  p1->y > p2->y ) {
        temp = *vx1;
        *vx1 = *vx2;
        *vx2 = temp;
    }

    if (  p2->y > p3->y ) {
        temp = *vx2;
        *vx2 = *vx3;
        *vx3 = temp;
    }

    if (  p1->y > p2->y ) {
        temp = *vx1;
        *vx1 = *vx2;
        *vx2 = temp;
    }
    */

}

static float _interpolate_scalar(float v1, float v2, float gradient) {
    return v1 + (v2 - v1) * gradient;
}

static color_t _interpolate_color(color_t* c1, color_t* c2, float gradient) {
    color_t r;
    r.r = (int)_interpolate_scalar((float)(c1->r), (float)(c2->r), gradient);
    r.g = (int)_interpolate_scalar((float)(c1->g), (float)(c2->g), gradient);
    r.b = (int)_interpolate_scalar((float)(c1->b), (float)(c2->b), gradient);
    r.a = (int)_interpolate_scalar((float)(c1->a), (float)(c2->a), gradient);
    return r;
}


static vector2f_t _interpolate_vector2f(vector2f_t* v1, vector2f_t* v2, float gradient) {
    vector2f_t r;
    r.x = _interpolate_scalar(v1->x, v2->x, gradient);
    r.y = _interpolate_scalar(v1->y, v2->y, gradient);
    return r;
}

static void _interpolate_row(vgpu_t* gpu, int y, 
        vertex_t* left_edge_vx1, vertex_t* left_edge_vx2,
        vertex_t* right_edge_vx1, vertex_t* right_edge_vx2
        ) {

    vector2_t* left_edge_p1 = left_edge_vx1->screen_pos; 
    vector2_t* left_edge_p2 = left_edge_vx2->screen_pos; 
    vector2_t* right_edge_p1 = right_edge_vx1->screen_pos;
    vector2_t* right_edge_p2 = right_edge_vx2->screen_pos;

    //Find gradient Y
    float left_gradient_y = 1.f;
    if (left_edge_p1->y != left_edge_p2->y) {
        left_gradient_y = (float)(y - left_edge_p1->y) / (float)(left_edge_p2->y - left_edge_p1->y);
    }

    float right_gradient_y = 1.f;
    if (right_edge_p1->y != right_edge_p2->y) {
        right_gradient_y = (float)(y - right_edge_p1->y) / (float)(right_edge_p2->y - right_edge_p1->y);
    }

    //Rasterize row
    int left_x  = (int)_interpolate_scalar((float)left_edge_p1->x, (float)left_edge_p2->x, left_gradient_y);
    int right_x = (int)_interpolate_scalar((float)right_edge_p1->x, (float)right_edge_p2->x, right_gradient_y);

    color_t left_color;
    color_t right_color;
    if (gpu->flags & VGPU_FLAG_COLOR) {
        left_color = _interpolate_color(left_edge_vx1->color, left_edge_vx2->color, left_gradient_y);
        right_color = _interpolate_color(right_edge_vx1->color, right_edge_vx2->color, right_gradient_y);
    }

    float left_z = _interpolate_scalar(left_edge_vx1->z_pos, left_edge_vx2->z_pos, left_gradient_y);
    float right_z = _interpolate_scalar(right_edge_vx1->z_pos, right_edge_vx2->z_pos, right_gradient_y);

    vector2f_t left_uv;
    vector2f_t right_uv;
    if (gpu->flags & VGPU_FLAG_TEXTURE) {
        left_uv = _interpolate_vector2f(left_edge_vx1->text_pos, left_edge_vx2->text_pos, left_gradient_y);
        right_uv = _interpolate_vector2f(right_edge_vx1->text_pos, right_edge_vx2->text_pos, right_gradient_y);
    }

    //ATTENZIONE alla X;
    for(int x = left_x; x <= right_x; ++x) {
        float gradient_x = 1.f;
        if (left_x < right_x) gradient_x = (float)(x - left_x) / (float)(right_x - left_x);
        float z = _interpolate_scalar(left_z, right_z, gradient_x);
        color_t color;
        
        if (gpu->flags & VGPU_FLAG_COLOR) {
            color = _interpolate_color(&left_color, &right_color, gradient_x);
        }

        if (gpu->flags & VGPU_FLAG_TEXTURE) {
            vector2f_t uv = _interpolate_vector2f(&left_uv, &right_uv, gradient_x);
            texture_t* text = gpu->texture;

            float uv_y_flipped = 1.f - uv.y;
            //attenzione: tenere in considerazione il "rolling" della uv
            int text_x =  (int)( (float)text->width * uv.x);
            int text_y =  (int)( (float)text->height * uv_y_flipped);
            int text_index = (text->width * text_y + text_x) * text->pixel_size;
            //TODO: Da migliorare e rendere dinamico rispetto al pixel size (memcpy?)
            color.r = text->data[text_index + 0];
            color.g = text->data[text_index + 1];
            color.b = text->data[text_index + 2];
            color.a = text->data[text_index + 3];
        }
        screen_put_pixel(gpu->screen, x, y, z, color);
    }
}

static void scanline_raster(vgpu_t* gpu, vertex_t* vx1, vertex_t* vx2, vertex_t* vx3) {
    //1) Sort vertices by y
    _sort_by_y(&vx1, &vx2, &vx3);
    
    //2) Calculate inversed slope dx/dy
    vector2_t* p1 = vx1->screen_pos;
    vector2_t* p2 = vx2->screen_pos;
    vector2_t* p3 = vx3->screen_pos;
    
    float inv_slope_p1p2 = (float)(p2->x - p1->x) / (float)(p2->y - p1->y);
    float inv_slope_p1p3 = (float)(p3->x - p1->x) / (float)(p3->y - p1->y);

    // <|
    if (inv_slope_p1p2 < inv_slope_p1p3) {
        for(int y = p1->y;  y <= p3->y; ++y) {
            //phase1: Upper Triangle: side left (p1, p2) side right (p1, p3)
            if (y < p2->y) {
                _interpolate_row(gpu, y,  vx1,vx2,  vx1,vx3);
            } else { //phase1: Lower Triangle: side left (p2, p3) side right (p1, p3)
                _interpolate_row(gpu, y,  vx2,vx3,  vx1,vx3);
            }
        }
    } else { // |>
        for(int y = p1->y;  y <= p3->y; ++y) {
            //phase1: Upper Triangle: side left (p1, p3) side right (p1, p2)
            if (y < p2->y) {
                _interpolate_row(gpu, y,  vx1,vx3,  vx1,vx2);
            } else { //phase1: Lower Triangle: side left (p1, p3) side right (p2, p3)
                _interpolate_row(gpu, y,  vx1,vx3,  vx2,vx3);
            }
        }
    }


}