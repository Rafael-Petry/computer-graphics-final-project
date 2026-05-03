#include <iostream>
#include <string>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "spaceship.h"
#include "../../vendor/include/matrices.h"

Spaceship::Spaceship(const std::string &meshPath, const glm::vec3 &color) : Object(meshPath, color) {}

glm::mat4 Spaceship::translate(float currentFrame) const { return Matrix_Translate(0.0f, 0.2f, 5.0f); }
glm::mat4 Spaceship::rotate(float currentFrame) const { return Matrix_Rotate_Y(currentFrame); }
glm::mat4 Spaceship::scale(float currentFrame) const { return Matrix_Scale(0.3f, 0.3f, 0.3f); }

void Spaceship::shoot() const { std::cout << "The spaceship is shooting..." << std::endl; }
