#pragma once

typedef struct texture_t texture_t;
typedef struct screen_t screen_t;

typedef enum vgpu_flags_e {
    VGPU_FLAG_TEXTURE = 1 << 1, 
    VGPU_FLAG_COLOR   = 1 << 2
} vgpu_flags_e;

typedef struct vgpu_t {
    screen_t* screen;
    texture_t* texture;
    vgpu_flags_e flags;
} vgpu_t;