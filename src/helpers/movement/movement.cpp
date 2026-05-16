#define TINYOBJLOADER_IMPLEMENTATION

#include "movement.h"

float MovementHelper::getMovementAxis(GLFWwindow *window, int positiveKey, int negativeKey)
{
    const bool positivePressed = glfwGetKey(window, positiveKey) == GLFW_PRESS;
    const bool negativePressed = glfwGetKey(window, negativeKey) == GLFW_PRESS;

    if (positivePressed == negativePressed) {
        return 0.0f;
    }

    return positivePressed ? 1.0f : -1.0f;
}

glm::vec3 MovementHelper::getMovementInputs(GLFWwindow *window)
{
    const float forward = getMovementAxis(window, GLFW_KEY_W, GLFW_KEY_S);
    const float right = getMovementAxis(window, GLFW_KEY_D, GLFW_KEY_A);
    const float up = getMovementAxis(window, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_LEFT_CONTROL);

    return glm::vec3(right, up, forward);
}