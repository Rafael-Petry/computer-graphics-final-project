#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

class MovementHelper
{
public:
    static glm::vec3 getMovementInputs(GLFWwindow *window);

private:
    static float getMovementAxis(GLFWwindow *window, int positiveKey, int negativeKey);
};

#endif
