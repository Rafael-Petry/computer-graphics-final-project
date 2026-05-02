#ifndef RENDER_H
#define RENDER_H

#include <string>

#include <glad/glad.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class RenderHelper
{
public:
    static void RenderModel(
        GLint modelUniform,
        GLint colorUniform,
        const glm::mat4& modelMatrix,
        const std::string& modelPath,
        const glm::vec3& color);
};

#endif
