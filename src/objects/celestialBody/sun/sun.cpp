#include <iostream>
#include <string>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "sun.h"
#include "../../../window/window.h"
#include "../../vendor/include/matrices.h"

Sun::Sun() : CelestialBody("../../src/objects/celestialBody/sun/sun.obj", glm::vec3(1.0f, 1.0f, 0.0f)) {}

Sun &Sun::getInstance()
{
    static Sun instance;
    return instance;
}

glm::mat4 Sun::translate(Window *window) const { return Matrix_Translate(0.0f, 0.0f, 0.0f); }
glm::mat4 Sun::rotate(Window *window) const { return Matrix_Rotate_Y(window->getCurrentFrame() * 0.1f); }
glm::mat4 Sun::scale(Window *window) const { return Matrix_Scale(1.1f, 1.1f, 1.1f); }
