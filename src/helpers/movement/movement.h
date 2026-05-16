#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../vendor/include/tiny_obj_loader.h"

class MovementHelper
{
public:
    static float getMovementAxis(GLFWwindow *window, int positiveKey, int negativeKey);
    static glm::vec3 getMovementInputs(GLFWwindow *window);
};

#endif
