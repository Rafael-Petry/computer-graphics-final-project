#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaders/loader/loader.h"
#include "window/window.h"

int main()
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    Window::getInstance().initialize(1280, 720, "Asteroids 3D");
    const GLuint shaderProgram = ShaderLoader::createShaderProgram("../../src/shaders/vertex.glsl", "../../src/shaders/fragment.glsl");

    while (!glfwWindowShouldClose(Window::getInstance().getWindow())) {
        Window::getInstance().update(shaderProgram);
    }

    Window::getInstance().close();
    glDeleteProgram(shaderProgram);
    return 0;
}
