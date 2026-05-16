#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>

#include "../helpers/render/render.h"
#include "object.h"
#include "../window/window.h"

Object::Object(std::string meshPath, glm::vec3 color) : meshPath(meshPath), color(color) {}

void Object::update(GLint modelUniform, GLint colorUniform, Window *window)
{
    const glm::mat4 model = translate(window) * rotate(window) * scale(window);
    RenderHelper::renderModel(modelUniform, colorUniform, model, meshPath, color);
}
