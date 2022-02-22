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
}obj_mesh_t;

static obj_mesh_t* obj_parser_parse(char* obj_path)
{
    FILE* file = fopen(obj_path, "r");
    if(file == NULL)
        return NULL;
    obj_mesh_t* mesh = malloc(sizeof(obj_mesh_t));

    char *buffer[1024];
    mesh->vertices_count = 0;
    mesh->vtexture_count = 0;

    while (fgets(buffer,1024,file))
    {
        if(!strncmp(buffer, "v ", 2)){
            ++mesh->vertices_count;
        }

        if (!strncmp(buffer, "vt ", 3))
        {
            ++mesh->vtexture_count;
        }
    }

    mesh->vertices_size = mesh->vertices_count*3;
    mesh->vtexture_size = mesh->vtexture_count*2;

    mesh->vertices = malloc(mesh->vertices_size*sizeof(float));
    mesh->vtexture = malloc(mesh->vtexture_size*sizeof(float));

    rewind(file);
    int v_index = 0;
    int vt_index = 0;

    while(fgets(buffer,1024,file))
    {
        char *token = strtok(buffer, " ");
        if(!strcmp(token,"v"))
        {
            token = strtok(NULL, " ");
            mesh->vertices[v_index + 0] = atof(token);

            token = strtok(NULL, " ");
            mesh->vertices[v_index + 1] = atof(token);

            token = strtok(NULL, " ");
            mesh->vertices[v_index + 2] = atof(token);
            v_index +=3;
        }

        if(!strcmp(token,"vt"))
        {
            token = strtok(NULL, " ");
            mesh->vtexture[vt_index + 0] = atof(token);

            token = strtok(NULL, " ");
            mesh->vtexture[vt_index + 1] = atof(token);
            vt_index += 2;
        }
    }

    fclose(file);
    return mesh;
}

static void obj_parser_free(obj_mesh_t* mesh_to_free)
{
    free(mesh_to_free);
}