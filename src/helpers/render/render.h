#ifndef RENDER_H
#define RENDER_H

#include <string>
#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.h"

class RenderHelper
{
public:
    static void renderModel(GLint modelUniform, GLint colorUniform, const glm::mat4 &modelMatrix, const Mesh &mesh, const glm::vec3 &color);
    static Mesh loadObjMesh(const std::string &path);
    static Mesh createCylinderMesh(int segments);
};

#endif
