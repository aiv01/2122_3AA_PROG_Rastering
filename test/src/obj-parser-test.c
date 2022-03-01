#define CLOVE_SUITE_NAME TestObjParser
#include "clove.h"
#include "obj-parser.h"

// CLOVE_EXEC_BASE_PATH path to test folder (parent of test.exe)
// CLOVE_EXEC_PATH path to .exe
CLOVE_TEST(ObjParserCreatingMeshFromExistingObjFile)
{
   obj_mesh_t *mesh = obj_parser_parse("bin\\test\\resources\\quad.obj");
   CLOVE_NOT_NULL(mesh);
   obj_parser_free(mesh);
}

CLOVE_TEST(ObjReadAllQuadVertex)
{
   obj_mesh_t *mesh = obj_parser_parse("bin\\test\\resources\\quad.obj");

   CLOVE_INT_EQ(mesh->vertices_count, 4);
   CLOVE_INT_EQ(mesh->vertices_size, 12);

   CLOVE_FLOAT_EQ(mesh->vertices[0], -1.f);
   CLOVE_FLOAT_EQ(mesh->vertices[1], 1.f);
   CLOVE_FLOAT_EQ(mesh->vertices[2], 0.f);

   CLOVE_FLOAT_EQ(mesh->vertices[3], 1.f);
   CLOVE_FLOAT_EQ(mesh->vertices[4], 1.f);
   CLOVE_FLOAT_EQ(mesh->vertices[5], 0.f);

   CLOVE_FLOAT_EQ(mesh->vertices[6], -1.f);
   CLOVE_FLOAT_EQ(mesh->vertices[7], -1.f);
   CLOVE_FLOAT_EQ(mesh->vertices[8], 0.f);

   CLOVE_FLOAT_EQ(mesh->vertices[9], 1.f);
   CLOVE_FLOAT_EQ(mesh->vertices[10], -1.f);
   CLOVE_FLOAT_EQ(mesh->vertices[11], 0.f);

   obj_parser_free(mesh);
}

CLOVE_TEST(ObjReadAllQuadVTexture)
{
   obj_mesh_t *mesh = obj_parser_parse("bin\\test\\resources\\quad.obj");

   CLOVE_INT_EQ(mesh->vtexture_count, 4);
   CLOVE_INT_EQ(mesh->vtexture_size, 8);

   CLOVE_FLOAT_EQ(mesh->vtexture[0], 1.f);
   CLOVE_FLOAT_EQ(mesh->vtexture[1], 1.f);

   CLOVE_FLOAT_EQ(mesh->vtexture[2], 0.f);
   CLOVE_FLOAT_EQ(mesh->vtexture[3], 1.f);

   CLOVE_FLOAT_EQ(mesh->vtexture[4], 0.f);
   CLOVE_FLOAT_EQ(mesh->vtexture[5], 0.f);

   CLOVE_FLOAT_EQ(mesh->vtexture[6], 1.f);
   CLOVE_FLOAT_EQ(mesh->vtexture[7], 0.f);

   obj_parser_free(mesh);
}

CLOVE_TEST(ObjReadAllQuadNormals)
{
   obj_mesh_t *mesh = obj_parser_parse("bin\\test\\resources\\quad.obj");

   CLOVE_INT_EQ(mesh->vnormals_count, 1);
   CLOVE_INT_EQ(mesh->vnormals_size, 3);

   CLOVE_FLOAT_EQ(mesh->vnormals[0], 0.f);
   CLOVE_FLOAT_EQ(mesh->vnormals[1], 0.f);
   CLOVE_FLOAT_EQ(mesh->vnormals[2], 1.f);

   obj_parser_free(mesh);
}

CLOVE_TEST(ReadAllQuadFaces)
{
   obj_mesh_t *mesh = obj_parser_parse("bin\\test\\resources\\quad.obj");

   CLOVE_INT_EQ(mesh->faces_count,2);
   CLOVE_INT_EQ(mesh->faces_size,18);

   CLOVE_INT_EQ(mesh->faces[0],4);
   CLOVE_INT_EQ(mesh->faces[1],4);
   CLOVE_INT_EQ(mesh->faces[2],1);

   CLOVE_INT_EQ(mesh->faces[3],1);
   CLOVE_INT_EQ(mesh->faces[4],2);
   CLOVE_INT_EQ(mesh->faces[5],1);

   CLOVE_INT_EQ(mesh->faces[6],3);
   CLOVE_INT_EQ(mesh->faces[7],3);
   CLOVE_INT_EQ(mesh->faces[8],1);

   CLOVE_INT_EQ(mesh->faces[9],4);
   CLOVE_INT_EQ(mesh->faces[10],4);
   CLOVE_INT_EQ(mesh->faces[11],1);

   CLOVE_INT_EQ(mesh->faces[12],2);
   CLOVE_INT_EQ(mesh->faces[13],1);
   CLOVE_INT_EQ(mesh->faces[14],1);

   CLOVE_INT_EQ(mesh->faces[15],1);
   CLOVE_INT_EQ(mesh->faces[16],2);
   CLOVE_INT_EQ(mesh->faces[17],1);

   obj_parser_free(mesh);
}

CLOVE_TEST(ReadAllQuadTriangle)
{
   obj_mesh_t *mesh = obj_parser_parse("bin\\test\\resources\\quad.obj");

   CLOVE_INT_EQ(mesh->triangles_count, 2);

   /* t1 v1 */
   CLOVE_FLOAT_EQ(mesh->triangles[0].v1.position.x, 1.f);
   CLOVE_FLOAT_EQ(mesh->triangles[0].v1.position.y, -1.f);
   CLOVE_FLOAT_EQ(mesh->triangles[0].v1.position.z, 0.f);
   CLOVE_FLOAT_EQ(mesh->triangles[0].v1.uv.x, 1.f);
   CLOVE_FLOAT_EQ(mesh->triangles[0].v1.uv.y, 0.f);
   CLOVE_FLOAT_EQ(mesh->triangles[0].v1.normal.x, 0.f);
   CLOVE_FLOAT_EQ(mesh->triangles[0].v1.normal.y, 0.f);
   CLOVE_FLOAT_EQ(mesh->triangles[0].v1.normal.z, 1.f);

   /* t1 v2 */
   CLOVE_FLOAT_EQ(mesh->triangles[0].v2.position.x, -1.f);
   CLOVE_FLOAT_EQ(mesh->triangles[0].v2.position.y, 1.f);
   CLOVE_FLOAT_EQ(mesh->triangles[0].v2.position.z, 0.f);
   CLOVE_FLOAT_EQ(mesh->triangles[0].v2.uv.x, 0.f);
   CLOVE_FLOAT_EQ(mesh->triangles[0].v2.uv.y, 1.f);
   CLOVE_FLOAT_EQ(mesh->triangles[0].v2.normal.x, 0.f);
   CLOVE_FLOAT_EQ(mesh->triangles[0].v2.normal.y, 0.f);
   CLOVE_FLOAT_EQ(mesh->triangles[0].v2.normal.z, 1.f);

   /* t1 v3 */
   CLOVE_FLOAT_EQ(mesh->triangles[0].v3.position.x, -1.f);
   CLOVE_FLOAT_EQ(mesh->triangles[0].v3.position.y, -1.f);
   CLOVE_FLOAT_EQ(mesh->triangles[0].v3.position.z, 0.f);
   CLOVE_FLOAT_EQ(mesh->triangles[0].v3.uv.x, 0.f);
   CLOVE_FLOAT_EQ(mesh->triangles[0].v3.uv.y, 0.f);
   CLOVE_FLOAT_EQ(mesh->triangles[0].v3.normal.x, 0.f);
   CLOVE_FLOAT_EQ(mesh->triangles[0].v3.normal.y, 0.f);
   CLOVE_FLOAT_EQ(mesh->triangles[0].v3.normal.z, 1.f);

   /* t2 v1 */
   CLOVE_FLOAT_EQ(mesh->triangles[1].v1.position.x, 1.f);
   CLOVE_FLOAT_EQ(mesh->triangles[1].v1.position.y, -1.f);
   CLOVE_FLOAT_EQ(mesh->triangles[1].v1.position.z, 0.f);
   CLOVE_FLOAT_EQ(mesh->triangles[1].v1.uv.x, 1.f);
   CLOVE_FLOAT_EQ(mesh->triangles[1].v1.uv.y, 0.f);
   CLOVE_FLOAT_EQ(mesh->triangles[1].v1.normal.x, 0.f);
   CLOVE_FLOAT_EQ(mesh->triangles[1].v1.normal.y, 0.f);
   CLOVE_FLOAT_EQ(mesh->triangles[1].v1.normal.z, 1.f);

   /* t2 v2 */
   CLOVE_FLOAT_EQ(mesh->triangles[1].v2.position.x, 1.f);
   CLOVE_FLOAT_EQ(mesh->triangles[1].v2.position.y, 1.f);
   CLOVE_FLOAT_EQ(mesh->triangles[1].v2.position.z, 0.f);
   CLOVE_FLOAT_EQ(mesh->triangles[1].v2.uv.x, 1.f);
   CLOVE_FLOAT_EQ(mesh->triangles[1].v2.uv.y, 1.f);
   CLOVE_FLOAT_EQ(mesh->triangles[1].v2.normal.x, 0.f);
   CLOVE_FLOAT_EQ(mesh->triangles[1].v2.normal.y, 0.f);
   CLOVE_FLOAT_EQ(mesh->triangles[1].v2.normal.z, 1.f);

   /* t2 v3 */
   CLOVE_FLOAT_EQ(mesh->triangles[1].v3.position.x, -1.f);
   CLOVE_FLOAT_EQ(mesh->triangles[1].v3.position.y, 1.f);
   CLOVE_FLOAT_EQ(mesh->triangles[1].v3.position.z, 0.f);
   CLOVE_FLOAT_EQ(mesh->triangles[1].v3.uv.x, 0.f);
   CLOVE_FLOAT_EQ(mesh->triangles[1].v3.uv.y, 1.f);
   CLOVE_FLOAT_EQ(mesh->triangles[1].v3.normal.x, 0.f);
   CLOVE_FLOAT_EQ(mesh->triangles[1].v3.normal.y, 0.f);
   CLOVE_FLOAT_EQ(mesh->triangles[1].v3.normal.z, 1.f);

   obj_parser_free(mesh);
}
