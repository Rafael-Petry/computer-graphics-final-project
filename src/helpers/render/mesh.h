#ifndef MESH_H
#define MESH_H

#include <vector>

#include <glad/glad.h>

#include "../../vendor/include/tiny_obj_loader.h"

struct Mesh
{
    GLuint vao = 0;
    GLuint vbo = 0;
    GLsizei vertexCount = 0;
    GLuint textureId = 0; // 0 = pas de texture
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
};

#endif