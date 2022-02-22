#include "vector.h"
#include <stdio.h>
#include <string.h>

typedef struct obj_mesh
{
    float *vertices;
    int vertices_size;
    int vertices_count;
}obj_mesh_t;

static obj_mesh_t* obj_parser_parse(char* obj_path)
{
    FILE* file = fopen(obj_path, "r");
    if(file == NULL)
        return NULL;
    obj_mesh_t* mesh = malloc(sizeof(obj_mesh_t));

    char *buffer[1024];
    int vertex_counter = 0;

    while (fgets(buffer,1024,file))
    {
        if(!strncmp(buffer, "v ", 2)){
            vertex_counter++;
        }
    }

    mesh->vertices_count = vertex_counter;
    mesh->vertices_size = vertex_counter*3;

    mesh->vertices = malloc(mesh->vertices_size*sizeof(float));
    rewind(file);
    int index = 0;


    while(fgets(buffer,1024,file))
    {

        char *token = strtok(buffer, " ");
        if(!strcmp(token,"v"))
        {
            token = strtok(NULL, " ");
            mesh->vertices[index + 0] = atof(token);

            token = strtok(NULL, " ");
            mesh->vertices[index + 1] = atof(token);

            token = strtok(NULL, " ");
            mesh->vertices[index + 2] = atof(token);
            index +=3;
        }
    }

    fclose(file);
    return mesh;
}

static void obj_parser_free(obj_mesh_t* mesh_to_free)
{
    free(mesh_to_free);
}