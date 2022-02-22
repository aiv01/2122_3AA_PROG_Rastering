#include "vector.h"
#include <stdio.h>
#include <string.h>

typedef struct obj_mesh
{
    float *vertices;
    int vertices_size;
    int vertices_count;

    float *vtexture;
    int vtexture_size;
    int vtexture_count;

    float *vnormals;
    int vnormals_size;
    int vnormals_count;
} obj_mesh_t;

static obj_mesh_t *obj_parser_parse(char *obj_path)
{
    FILE *file = NULL;
    if (fopen_s(&file, obj_path, "r") != 0)
        return NULL;

    obj_mesh_t *mesh = malloc(sizeof(obj_mesh_t));

    char buffer[1024];
    mesh->vertices_count = 0;
    mesh->vtexture_count = 0;
    mesh->vnormals_count = 0;

    while (fgets(buffer, 1024, file))
    {
        if (!strncmp(buffer, "v ", 2))
        {
            ++mesh->vertices_count;
        }

        if (!strncmp(buffer, "vt ", 3))
        {
            ++mesh->vtexture_count;
        }

        if (!strncmp(buffer, "vn ", 3))
        {
            ++mesh->vnormals_count;
        }
    }

    mesh->vertices_size = mesh->vertices_count * 3;
    mesh->vtexture_size = mesh->vtexture_count * 2;
    mesh->vnormals_size = mesh->vnormals_count * 3;

    mesh->vertices = malloc(mesh->vertices_size * sizeof(float));
    mesh->vtexture = malloc(mesh->vtexture_size * sizeof(float));
    mesh->vnormals = malloc(mesh->vnormals_size * sizeof(float));

    rewind(file);
    int v_index = 0;
    int vt_index = 0;
    int vn_index = 0;

    while (fgets(buffer, 1024, file))
    {
        char *context;
        char *token = strtok_s(buffer, " ", &context);
        if (!strcmp(token, "v"))
        {
            token = strtok_s(NULL, " ", &context);
            mesh->vertices[v_index + 0] = atof(token);

            token = strtok_s(NULL, " ", &context);
            mesh->vertices[v_index + 1] = atof(token);

            token = strtok_s(NULL, " ", &context);
            mesh->vertices[v_index + 2] = atof(token);
            v_index += 3;
        }

        if (!strcmp(token, "vt"))
        {
            token = strtok_s(NULL, " ", &context);
            mesh->vtexture[vt_index + 0] = atof(token);

            token = strtok_s(NULL, " ", &context);
            mesh->vtexture[vt_index + 1] = atof(token);
            vt_index += 2;
        }

        if (!strcmp(token, "vn"))
        {
            token = strtok_s(NULL, " ", &context);
            mesh->vnormals[vn_index + 0] = atof(token);

            token = strtok_s(NULL, " ", &context);
            mesh->vnormals[vn_index + 1] = atof(token);

            token = strtok_s(NULL, " ", &context);
            mesh->vnormals[vn_index + 2] = atof(token);
            vn_index += 3;
        }
    }

    fclose(file);
    return mesh;
}

static void obj_parser_free(obj_mesh_t *mesh_to_free)
{
    free(mesh_to_free);
}