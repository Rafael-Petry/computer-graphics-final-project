#include <iostream>
#include <string>

#include <glm/mat4x4.hpp>

#include "planet.h"
#include "../../../window/window.h"
#include "../../vendor/include/matrices.h"

Planet::Planet(const std::string &meshPath, const glm::vec3 &color) : CelestialBody(meshPath, color) {}

glm::mat4 Planet::translate(Window *window) const { return Matrix_Translate(2.0f * cosf(window->getCurrentFrame() * 0.4f), 0.0f, 2.0f * sinf(window->getCurrentFrame() * 0.4f)); }
glm::mat4 Planet::rotate(Window *window) const { return Matrix_Rotate_Y(window->getCurrentFrame() * 1.7f); }
glm::mat4 Planet::scale(Window *window) const { return Matrix_Scale(0.6f, 0.6f, 0.6f); }
