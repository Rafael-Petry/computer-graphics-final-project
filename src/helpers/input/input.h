#ifndef INPUT_H
#define INPUT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>

class InputHelper
{
public:
	static void ProcessCommonInput(GLFWwindow* window);
	static float GetAxis(GLFWwindow* window, int positiveKey, int negativeKey);
	static glm::vec3 GetCameraMovement(GLFWwindow* window);
};

#endif
