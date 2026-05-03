#ifndef RENDER_H
#define RENDER_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

#include <glad/glad.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../vendor/include/tiny_obj_loader.h"

class RenderHelper
{
public:
    static void renderModel(GLint modelUniform, GLint colorUniform, const glm::mat4 &modelMatrix, const std::string &modelPath, const glm::vec3 &color);
};

#endif
