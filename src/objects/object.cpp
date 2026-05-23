#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>

#include "../helpers/render/render.h"
#include "object.h"
#include "../helpers/collision/colliders/collider.h"
#include "../window/window.h"
#include "../../vendor/include/matrices.h"

Object::Object(const Mesh &mesh, const Collider &collider, const glm::vec3 &color) : mesh(mesh), collider(collider), color(color) {}

void Object::update(GLint modelUniform, GLint colorUniform, Window *window)
{
    const glm::mat4 model = translate(window) * rotate(window) * scale(window) * Matrix_Translate(0.0f, 0.0f, 0.0f);
    RenderHelper::renderModel(modelUniform, colorUniform, model, mesh, color);
}

const Mesh &Object::getMesh() const { return mesh; }

const Collider &Object::getCollider() const { return collider; }
