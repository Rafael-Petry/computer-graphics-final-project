#include <iostream>
#include <string>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "spaceship.h"
#include "../../helpers/movement/movement.h"
#include "../../window/window.h"
#include "../../vendor/include/matrices.h"

Spaceship::Spaceship(const std::string &meshPath, const glm::vec3 &color) : Object(meshPath, color)
{
    front = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    right = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
    worldUp = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    position = glm::vec4(0.0f, 0.2f, 5.0f, 1.0f);
}

void Spaceship::update(GLint modelUniform, GLint colorUniform, Window *window)
{
    Object::update(modelUniform, colorUniform, window);

    isRolling = false;
    if (glfwGetKey(window->getGlfwWindow(), GLFW_KEY_R) == GLFW_PRESS) {
        isRolling = true;
    }
}

void Spaceship::updateView(GLFWwindow *window, double xpos, double ypos)
{
    Spaceship *spaceship = static_cast<Spaceship *>(glfwGetWindowUserPointer(window));

    if (spaceship != nullptr) {
        if (spaceship->firstMouseUpdate) {
            spaceship->lastMouseX = static_cast<float>(xpos);
            spaceship->lastMouseY = static_cast<float>(ypos);
            spaceship->firstMouseUpdate = false;
        }

        float xoffset = static_cast<float>(xpos) - spaceship->lastMouseX;
        float yoffset = spaceship->lastMouseY - static_cast<float>(ypos);

        spaceship->lastMouseX = static_cast<float>(xpos);
        spaceship->lastMouseY = static_cast<float>(ypos);

        xoffset *= spaceship->mouseSensitivity;
        yoffset *= spaceship->mouseSensitivity;

        if (spaceship->isRolling) {
            spaceship->roll += xoffset;
        }
        else {
            spaceship->yaw += xoffset;
            spaceship->pitch += yoffset;

            if (spaceship->pitch > 89.0f) {
                spaceship->pitch = 89.0f;
            }

            if (spaceship->pitch < -89.0f) {
                spaceship->pitch = -89.0f;
            }
        }

        spaceship->updateOrientation();
    }
}

void Spaceship::shoot() const { std::cout << "The spaceship is shooting..." << std::endl; }

void Spaceship::updateOrientation()
{
    const float yawRad = yaw * (M_PI / 180.0f);
    const float pitchRad = pitch * (M_PI / 180.0f);
    const float rollRad = roll * (M_PI / 180.0f);

    glm::vec4 newFront;
    newFront.x = cosf(yawRad) * cosf(pitchRad);
    newFront.y = sinf(pitchRad);
    newFront.z = sinf(yawRad) * cosf(pitchRad);
    newFront.w = 0.0f;

    front = (newFront / norm(newFront));
    right = (crossproduct(front, worldUp)) / norm(crossproduct(front, worldUp));
    up = (crossproduct(right, front)) / norm(crossproduct(right, front));

    if (rollRad != 0.0f) {
        const glm::mat4 rollMatrix = Matrix_Rotate(rollRad, front);
        const glm::vec4 rolledRight = rollMatrix * right;
        right = rolledRight / norm(rolledRight);
        up = crossproduct(right, front) / norm(crossproduct(right, front));
    }
}

glm::mat4 Spaceship::translate(Window *window)
{
    const float velocity = movementSpeed * window->getDeltaTime();
    const glm::vec3 movement = MovementHelper::getMovementInputs(window->getGlfwWindow());

    position += front * (movement.z * velocity);
    position += right * (movement.x * velocity);
    position += up * (movement.y * velocity);

    return Matrix_Translate(position.x, position.y, position.z);
}

glm::mat4 Spaceship::rotate(Window *window)
{
    const glm::vec3 forward = -front;
    return Matrix(right.x, up.x, forward.x, 0.0f, right.y, up.y, forward.y, 0.0f, right.z, up.z, forward.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

glm::mat4 Spaceship::scale(Window *window) { return Matrix_Scale(0.3f, 0.3f, 0.3f); }

glm::mat4 Spaceship::getViewMatrix() const { return Matrix_cameraView(position + front, front, up); }
