#include <iostream>
#include <string>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "spaceship.h"
#include "../../vendor/include/matrices.h"

Spaceship::Spaceship() : Object("../../src/objects/spaceship/spaceship.obj", glm::vec3(0.73f, 0.79f, 0.88f)) {}

glm::mat4 Spaceship::translate(float currentFrame) const { return Matrix_Translate(0.0f, 0.2f, 5.0f); }

glm::mat4 Spaceship::rotate(float currentFrame) const { return Matrix_Rotate_Y(currentFrame); }

void Spaceship::shoot() const { std::cout << "The spaceship is shooting..." << std::endl; }
