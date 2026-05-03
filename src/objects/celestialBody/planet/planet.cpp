#include <iostream>
#include <string>

#include <glm/mat4x4.hpp>

#include "planet.h"
#include "../../vendor/include/matrices.h"

Planet::Planet(const std::string &meshPath, const glm::vec3 &color) : CelestialBody(meshPath, color) {}

glm::mat4 Planet::translate(float currentFrame) const { return Matrix_Translate(2.0f * cosf(currentFrame * 0.4f), 0.0f, 2.0f * sinf(currentFrame * 0.4f)); }
glm::mat4 Planet::rotate(float currentFrame) const { return Matrix_Rotate_Y(currentFrame * 1.7f); }
glm::mat4 Planet::scale(float currentFrame) const { return Matrix_Scale(0.6f, 0.6f, 0.6f); }
