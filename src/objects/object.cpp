#include <string>

#include <glad/glad.h>
#include <glm/vec3.hpp>

#include "../helpers/render/render.h"
#include "object.h"

Object::Object(std::string meshPath, glm::vec3 color) : meshPath(meshPath), color(color) {}

void Object::update(GLint modelUniform, GLint colorUniform, float currentFrame) const
{
    const glm::mat4 model = translate(currentFrame) * rotate(currentFrame) * scale(currentFrame);
    RenderHelper::renderModel(modelUniform, colorUniform, model, meshPath, color);
}
