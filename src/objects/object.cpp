#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>

#include "../helpers/render/render.h"
#include "object.h"
#include "../window/window.h"
#include "../../vendor/include/matrices.h"

Object::Object(std::string meshPath, glm::vec3 color) : meshPath(meshPath), color(color) {}

void Object::update(GLint modelUniform, GLint colorUniform, Window *window)
{
    const glm::mat4 model = translate(window) * rotate(window) * scale(window) * Matrix_Translate(0.0f, 0.0f, 0.0f);
    RenderHelper::renderModel(modelUniform, colorUniform, model, meshPath, color);
}
