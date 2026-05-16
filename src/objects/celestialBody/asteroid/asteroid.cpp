#include <iostream>
#include <string>

#include "asteroid.h"
#include "../../../window/window.h"
#include "../../vendor/include/matrices.h"

Asteroid::Asteroid(const std::string &meshPath, const glm::vec3 &color) : CelestialBody(meshPath, color) {}

glm::mat4 Asteroid::translate(Window *window) const { return Matrix_Translate(2.7f * cosf(window->getCurrentFrame() * 0.8f), 0.4f, 2.7f * sinf(window->getCurrentFrame() * 0.8f)); }
glm::mat4 Asteroid::rotate(Window *window) const { return Matrix_Rotate_Y(window->getCurrentFrame() * 1.7f); }
glm::mat4 Asteroid::scale(Window *window) const { return Matrix_Scale(0.1f, 0.1f, 0.1f); }
