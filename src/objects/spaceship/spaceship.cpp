#include <iostream>
#include <string>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "spaceship.h"
#include <GLFW/glfw3.h>
#include "../../vendor/include/matrices.h"

namespace {
    float getMovementAxis(GLFWwindow *window, int positiveKey, int negativeKey)
    {
        const bool positivePressed = glfwGetKey(window, positiveKey) == GLFW_PRESS;
        const bool negativePressed = glfwGetKey(window, negativeKey) == GLFW_PRESS;

        if (positivePressed == negativePressed) {
            return 0.0f;
        }

        return positivePressed ? 1.0f : -1.0f;
    }

    glm::vec3 getMovementInputs(GLFWwindow *window)
    {
        const float forward = getMovementAxis(window, GLFW_KEY_W, GLFW_KEY_S);
        const float right = getMovementAxis(window, GLFW_KEY_D, GLFW_KEY_A);
        const float up = getMovementAxis(window, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_LEFT_CONTROL);

        return glm::vec3(right, up, forward);
    }
} // input namespace

Spaceship::Spaceship(const std::string &meshPath, const glm::vec3 &color) : Object(meshPath, color) {}

glm::mat4 Spaceship::translate(float currentFrame) const
{
    /*
    const float velocity = movementSpeed * deltaTime;
    const glm::vec3 movement = getMovementInputs(window);

    position += front * (movement.z * velocity);
    position += right * (movement.x * velocity);
    position += worldUp * (movement.y * velocity);
    */

    return Matrix_Translate(0.0f, 0.2f, 5.0f);
}

glm::mat4 Spaceship::rotate(float currentFrame) const { return Matrix_Rotate_Y(currentFrame); }
glm::mat4 Spaceship::scale(float currentFrame) const { return Matrix_Scale(0.3f, 0.3f, 0.3f); }

void Spaceship::shoot() const { std::cout << "The spaceship is shooting..." << std::endl; }
