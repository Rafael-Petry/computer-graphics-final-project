#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>

#include "../helpers/render/render.h"
#include "object.h"
#include "../helpers/collision/colliders/boundingBox.h"
#include "../window/window.h"
#include "../../vendor/include/matrices.h"

Object::Object(const Mesh &mesh, const BoundingBox &boundingBox, const glm::vec3 &color) : mesh(mesh), boundingBox(boundingBox), color(color) {}

void Object::update(GLint modelUniform, GLint colorUniform, Window *window)
{
    const glm::mat4 model = translate(window) * rotate(window) * scale(window) * Matrix_Translate(0.0f, 0.0f, 0.0f);
    RenderHelper::renderModel(modelUniform, colorUniform, model, mesh, color);
}

const Mesh &Object::getMesh() const { return mesh; }

const BoundingBox &Object::getBoundingBox() const { return boundingBox; }
