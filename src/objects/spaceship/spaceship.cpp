#include <iostream>
#include <string>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "spaceship.h"
#include "../../helpers/movement/movement.h"
#include "../../window/window.h"
#include "../../vendor/include/matrices.h"

Spaceship::Spaceship(const std::string &meshPath, const glm::vec3 &color) : Object(meshPath, color)
{
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    right = glm::vec3(1.0f, 0.0f, 0.0f);
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    position = glm::vec3(0.0f, 0.2f, 5.0f);
}

glm::mat4 Spaceship::translate(Window *window)
{
    const float velocity = movementSpeed * window->getDeltaTime();
    const glm::vec3 movement = MovementHelper::getMovementInputs(window->getGlfwWindow());

    position += front * (movement.z * velocity);
    position += right * (movement.x * velocity);
    position += worldUp * (movement.y * velocity);

    return Matrix_Translate(position.x, position.y, position.z);
}

glm::mat4 Spaceship::rotate(Window *window) { return Matrix_Rotate_Y(window->getCurrentFrame()); }
glm::mat4 Spaceship::scale(Window *window) { return Matrix_Scale(0.3f, 0.3f, 0.3f); }

void Spaceship::shoot() const { std::cout << "The spaceship is shooting..." << std::endl; }
