#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

struct Mesh
{
    GLuint vao = 0;
    GLuint vbo = 0;
    GLsizei vertexCount = 0;
};

#endif
