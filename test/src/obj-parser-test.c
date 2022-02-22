#define CLOVE_SUITE_NAME TestObjParser
#include "clove.h"
#include "obj-parser.h"

//CLOVE_EXEC_BASE_PATH path to test folder (parent of test.exe)
//CLOVE_EXEC_PATH path to .exe
CLOVE_TEST(ObjParserCreatingMeshFromExistingObjFile) {
   obj_mesh_t* mesh = obj_parser_parse("bin\\test\\resources\\quad.obj");
   CLOVE_NOT_NULL(mesh);
   obj_parser_free(mesh);
}

CLOVE_TEST(ObjReadAllQuadVertex)
{
   obj_mesh_t* mesh = obj_parser_parse("bin\\test\\resources\\quad.obj");

   CLOVE_INT_EQ(mesh->vertices_count, 4);
   CLOVE_INT_EQ(mesh->vertices_size, 12);

   CLOVE_FLOAT_EQ(mesh->vertices[0],-1.f);
   CLOVE_FLOAT_EQ(mesh->vertices[1],1.f);
   CLOVE_FLOAT_EQ(mesh->vertices[2],0.f);

   CLOVE_FLOAT_EQ(mesh->vertices[3],1.f);
   CLOVE_FLOAT_EQ(mesh->vertices[4],1.f);
   CLOVE_FLOAT_EQ(mesh->vertices[5],0.f);

   CLOVE_FLOAT_EQ(mesh->vertices[6],-1.f);
   CLOVE_FLOAT_EQ(mesh->vertices[7],-1.f);
   CLOVE_FLOAT_EQ(mesh->vertices[8],0.f);

   CLOVE_FLOAT_EQ(mesh->vertices[9],1.f);
   CLOVE_FLOAT_EQ(mesh->vertices[10],-1.f);
   CLOVE_FLOAT_EQ(mesh->vertices[11],0.f); 

   obj_parser_free(mesh);
}
