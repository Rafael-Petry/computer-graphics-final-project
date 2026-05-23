#include <iostream>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "sun.h"
#include "../../../helpers/render/render.h"
#include "../../../window/window.h"
#include "../../vendor/include/matrices.h"

Mesh Sun::mesh;

Sun::Sun() : CelestialBody(mesh, glm::vec3(1.0f, 1.0f, 0.0f)), position(0.0f, 0.0f, 0.0f)
{
    if (mesh.vao == 0) {
        mesh = RenderHelper::loadObjMesh("../../src/objects/celestialBody/sun/sun.obj");
    }
}

Sun &Sun::getInstance()
{
    static Sun instance;
    return instance;
}

glm::vec3 Sun::getPosition() const { return position; }

glm::mat4 Sun::translate(Window *window) { return Matrix_Translate(position.x, position.y, position.z); }
glm::mat4 Sun::rotate(Window *window) { return Matrix_Rotate_Y(window->getCurrentFrame() * 0.1f); }
glm::mat4 Sun::scale(Window *window) { return Matrix_Scale(1.1f, 1.1f, 1.1f); }
