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

