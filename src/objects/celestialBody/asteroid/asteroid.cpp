#include <iostream>
#include <string>

#include "asteroid.h"
#include "../../vendor/include/matrices.h"

Asteroid::Asteroid(const std::string &meshPath, const glm::vec3 &color) : CelestialBody(meshPath, color) {}

glm::mat4 Asteroid::translate(float currentFrame) const { return Matrix_Translate(2.7f * cosf(currentFrame * 0.8f), 0.4f, 2.7f * sinf(currentFrame * 0.8f)); }
glm::mat4 Asteroid::rotate(float currentFrame) const { return Matrix_Rotate_Y(currentFrame * 1.7f); }
glm::mat4 Asteroid::scale(float currentFrame) const { return Matrix_Scale(0.1f, 0.1f, 0.1f); }
