#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct obj_float3_t
{
    float x;
    float y;
    float z;
} obj_float3_t;

typedef struct obj_float2_t
{
    float x;
    float y;
} obj_float2_t;

typedef struct obj_vertex_t
{
    obj_float3_t position;
    obj_float3_t normal;
    obj_float2_t uv;
} obj_vertex_t;

typedef struct obj_triangle_t
{
    obj_vertex_t v1;
    obj_vertex_t v2;
    obj_vertex_t v3;
} obj_triangle_t;

typedef struct obj_mesh_t
{
    float *vertices;
    int vertices_size;
    int vertices_count;

    float *uvs;
    int uvs_size;
    int uvs_count;

    float *normals;
    int normals_size;
    int normals_count;

    int *faces;
    int faces_count;
    int faces_size;

    int triangles_count;
    obj_triangle_t *triangles;
} obj_mesh_t;


void obj_parser_free(obj_mesh_t *mesh);
obj_mesh_t *obj_parser_parse(char *obj_path);

#ifdef OBJ_PARSER_IMPLEMENTATION
obj_mesh_t *obj_parser_parse(char *obj_path)
{
    FILE *file = NULL;
    if (fopen_s(&file, obj_path, "r") != 0)
        return NULL;

    obj_mesh_t *mesh = (obj_mesh_t*)malloc(sizeof(obj_mesh_t));

    char buffer[1024];
    mesh->vertices_count = 0;
    mesh->uvs_count = 0;
    mesh->normals_count = 0;
    mesh->faces_count = 0;

    while (fgets(buffer, 1024, file))
    {
        if (!strncmp(buffer, "v ", 2))
        {
            ++mesh->vertices_count;
        }

        if (!strncmp(buffer, "vt ", 3))
        {
            ++mesh->uvs_count;
        }

        if (!strncmp(buffer, "vn ", 3))
        {
            ++mesh->normals_count;
        }

        if (!strncmp(buffer, "f ", 2))
        {
            ++mesh->faces_count;
        }
    }

    mesh->vertices_size = mesh->vertices_count * 3;
    mesh->uvs_size = mesh->uvs_count * 2;
    mesh->normals_size = mesh->normals_count * 3;
    mesh->faces_size = mesh->faces_count * 9;

    mesh->vertices = malloc(mesh->vertices_size * sizeof(float));
    mesh->uvs = malloc(mesh->uvs_size * sizeof(float));
    mesh->normals = malloc(mesh->normals_size * sizeof(float));
    mesh->faces = malloc(mesh->faces_size * sizeof(int));

    rewind(file);
    int v_index = 0;
    int vt_index = 0;
    int vn_index = 0;
    int f_index = 0;

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
            mesh->uvs[vt_index + 0] = atof(token);

            token = strtok_s(NULL, " ", &context);
            mesh->uvs[vt_index + 1] = atof(token);
            vt_index += 2;
        }

        if (!strcmp(token, "vn"))
        {
            token = strtok_s(NULL, " ", &context);
            mesh->normals[vn_index + 0] = atof(token);

            token = strtok_s(NULL, " ", &context);
            mesh->normals[vn_index + 1] = atof(token);

            token = strtok_s(NULL, " ", &context);
            mesh->normals[vn_index + 2] = atof(token);
            vn_index += 3;
        }

        if (!strcmp(token, "f"))
        {
            for (size_t i = 0; i < 3; i++)
            {

                token = strtok_s(NULL, "/", &context);
                mesh->faces[f_index + 0] = atoi(token);

                token = strtok_s(NULL, "/", &context);
                mesh->faces[f_index + 1] = atoi(token);

                token = strtok_s(NULL, " ", &context);
                mesh->faces[f_index + 2] = atoi(token);
                f_index += 3;
            }
        }
    }

    fclose(file);

    mesh->triangles_count = mesh->faces_count;
    mesh->triangles = malloc(sizeof(obj_triangle_t) * mesh->triangles_count);
    int vertex_index;
    int uv_index;
    int normal_index;

    for(int i = 0; i < mesh->triangles_count; i++)
    {
        vertex_index = (mesh->faces[i * 9 + 0] - 1) * 3;
        mesh->triangles[i].v1.position.x = mesh->vertices[vertex_index + 0];
        mesh->triangles[i].v1.position.y = mesh->vertices[vertex_index + 1];
        mesh->triangles[i].v1.position.z = mesh->vertices[vertex_index + 2];

        uv_index = (mesh->faces[i * 9 + 1] - 1) * 2;
        mesh->triangles[i].v1.uv.x = mesh->uvs[uv_index + 0];
        mesh->triangles[i].v1.uv.y = mesh->uvs[uv_index + 1];

        normal_index = (mesh->faces[i * 9 + 2] - 1) * 3;
        mesh->triangles[i].v1.normal.x = mesh->normals[normal_index + 0];
        mesh->triangles[i].v1.normal.y = mesh->normals[normal_index + 1];
        mesh->triangles[i].v1.normal.z = mesh->normals[normal_index + 2];

        vertex_index = (mesh->faces[i * 9 + 3] - 1) * 3;
        mesh->triangles[i].v2.position.x = mesh->vertices[vertex_index + 0];
        mesh->triangles[i].v2.position.y = mesh->vertices[vertex_index + 1];
        mesh->triangles[i].v2.position.z = mesh->vertices[vertex_index + 2];

        uv_index = (mesh->faces[i * 9 + 4] - 1) * 2;
        mesh->triangles[i].v2.uv.x = mesh->uvs[uv_index + 0];
        mesh->triangles[i].v2.uv.y = mesh->uvs[uv_index + 1];

        normal_index = (mesh->faces[i * 9 + 5] - 1) * 3;
        mesh->triangles[i].v2.normal.x = mesh->normals[normal_index + 0];
        mesh->triangles[i].v2.normal.y = mesh->normals[normal_index + 1];
        mesh->triangles[i].v2.normal.z = mesh->normals[normal_index + 2];

        vertex_index = (mesh->faces[i * 9 + 6] - 1) * 3;
        mesh->triangles[i].v3.position.x = mesh->vertices[vertex_index + 0];
        mesh->triangles[i].v3.position.y = mesh->vertices[vertex_index + 1];
        mesh->triangles[i].v3.position.z = mesh->vertices[vertex_index + 2];

        uv_index = (mesh->faces[i * 9 + 7] - 1) * 2;
        mesh->triangles[i].v3.uv.x = mesh->uvs[uv_index + 0];
        mesh->triangles[i].v3.uv.y = mesh->uvs[uv_index + 1];

        normal_index = (mesh->faces[i * 9 + 8] - 1) * 3;
        mesh->triangles[i].v3.normal.x = mesh->normals[normal_index + 0];
        mesh->triangles[i].v3.normal.y = mesh->normals[normal_index + 1];
        mesh->triangles[i].v3.normal.z = mesh->normals[normal_index + 2];
    }

    return mesh;
}

void obj_parser_free(obj_mesh_t *mesh)
{   
    free(mesh->vertices);
    free(mesh->uvs);
    free(mesh->normals);
    free(mesh->faces);
    free(mesh);
}

#endif //OBJ_PARSER_IMPLEMENTATION