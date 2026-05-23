#ifndef RENDER_H
#define RENDER_H

#include <unordered_map>
#include <string>

#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.h"

class RenderHelper
{
public:
    static void renderModel(GLint modelUniform, GLint colorUniform, const glm::mat4 &modelMatrix, const std::string &modelPath, const glm::vec3 &color);

private:
    static std::unordered_map<std::string, Mesh> cache;

    static Mesh loadObjMesh(const std::string &path);
    static const Mesh &getMesh(const std::string &path);
};

#endif
