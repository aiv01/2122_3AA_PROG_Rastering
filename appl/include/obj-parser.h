#include "vector.h"
#include <stdio.h>

typedef struct obj_mesh
{
    int a;
}obj_mesh_t;

static obj_mesh_t* obj_parser_parse(char* obj_path)
{
    FILE* file = fopen(obj_path, "r");
    if(file == NULL)
        return NULL;
    obj_mesh_t* mesh = malloc(sizeof(obj_mesh_t));
    return mesh;
}

static void obj_parser_free(obj_mesh_t* mesh_to_free)
{
    free(mesh_to_free);
}