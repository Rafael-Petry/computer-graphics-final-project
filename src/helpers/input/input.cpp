#include "input.h"

void InputHelper::getWindowInputs(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

glm::vec3 InputHelper::getMovementInputs(GLFWwindow *window)
{
    const float forward = getAxis(window, GLFW_KEY_W, GLFW_KEY_S);
    const float right = getAxis(window, GLFW_KEY_D, GLFW_KEY_A);
    const float up = getAxis(window, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_LEFT_CONTROL);

    return glm::vec3(right, up, forward);
}

float InputHelper::getAxis(GLFWwindow *window, int positiveKey, int negativeKey)
{
    const bool positivePressed = glfwGetKey(window, positiveKey) == GLFW_PRESS;
    const bool negativePressed = glfwGetKey(window, negativeKey) == GLFW_PRESS;

    if (positivePressed == negativePressed) {
        return 0.0f;
    }

    return positivePressed ? 1.0f : -1.0f;
}
