#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>

#include "../helpers/render/render.h"
#include "object.h"
#include "../helpers/collision/colliders/collider.h"
#include "../window/window.h"
#include "../../vendor/include/matrices.h"

Object::Object(const Mesh &mesh, const Collider &collider, const glm::vec3 &color) : mesh(mesh), collider(collider), color(color), scaleValue(1.0f), position(0.0f) {}

void Object::update(GLint modelUniform, GLint colorUniform, Window *window, GLint useTextureUniform, GLint texSamplerUniform, GLint isEmissiveUniform, bool isEmissive)
{
    if (isEmissiveUniform >= 0)
        glUniform1i(isEmissiveUniform, isEmissive ? 1 : 0);

    const glm::mat4 model = translate(window) * rotate(window) * scale(window) * Matrix_Translate(0.0f, 0.0f, 0.0f);
    RenderHelper::renderModelTextured(modelUniform, colorUniform, useTextureUniform, texSamplerUniform, model, mesh, color);

    if (isEmissiveUniform >= 0)
        glUniform1i(isEmissiveUniform, 0); // reset après le rendu

    collide(window);
}

const Mesh &Object::getMesh() const { return mesh; }

const Collider &Object::getCollider() const { return collider; }

const glm::vec3 &Object::getScale() const { return scaleValue; }

const glm::vec3 &Object::getPosition() const { return position; }

void Object::setPosition(const glm::vec3 &newPosition) { position = newPosition; }

glm::mat4 Object::scale(Window *window)
{
    (void)window;
    return Matrix_Scale(scaleValue.x, scaleValue.y, scaleValue.z);
}

void Object::collide(Window *window) { (void)window; }