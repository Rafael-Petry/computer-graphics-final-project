#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

class InputHelper
{

public:
    static void getWindowInputs(GLFWwindow *window);
    static glm::vec3 getMovementInputs(GLFWwindow *window);

private:
    static float getAxis(GLFWwindow *window, int positiveKey, int negativeKey);
};

#endif
