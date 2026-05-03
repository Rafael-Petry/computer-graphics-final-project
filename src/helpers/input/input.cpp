#include "input.h"

void InputHelper::ListenWindowInputs(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

float InputHelper::GetAxis(GLFWwindow *window, int positiveKey, int negativeKey)
{
    const bool positivePressed = glfwGetKey(window, positiveKey) == GLFW_PRESS;
    const bool negativePressed = glfwGetKey(window, negativeKey) == GLFW_PRESS;

    if (positivePressed == negativePressed) {
        return 0.0f;
    }

    return positivePressed ? 1.0f : -1.0f;
}

glm::vec3 InputHelper::GetCameraMovement(GLFWwindow *window)
{
    const float forward = GetAxis(window, GLFW_KEY_W, GLFW_KEY_S);
    const float right = GetAxis(window, GLFW_KEY_D, GLFW_KEY_A);
    const float up = GetAxis(window, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_LEFT_CONTROL);

    return glm::vec3(right, up, forward);
}
