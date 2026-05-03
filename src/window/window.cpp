#include <iostream>
#include <cmath>
#include <string>
#include <utility>

#include "window.h"

Window &Window::getInstance()
{
    static Window instance;
    return instance;
}

void Window::framebufferSizeCallback(GLFWwindow *glfwWindow, int width, int height)
{
    glViewport(0, 0, width, height);

    Camera *camera = static_cast<Camera *>(glfwGetWindowUserPointer(glfwWindow));
    if (camera != nullptr) {
        camera->updateAspectRatio(width, height);
    }
}

bool Window::initialize(int width, int height, std::string title)
{
    this->title = std::move(title);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    glfwWindow = glfwCreateWindow(width, height, this->title.c_str(), nullptr, nullptr);
    if (glfwWindow == nullptr) {
        std::cerr << "Failed to create GLFW glfwWindow." << std::endl;
        close();
        return false;
    }

    glfwMakeContextCurrent(glfwWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD." << std::endl;
        close();
        return false;
    }

    int framebufferWidth = 0;
    int framebufferHeight = 0;
    glfwGetFramebufferSize(glfwWindow, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);

    const float aspectRatio = static_cast<float>(framebufferWidth) / static_cast<float>(framebufferHeight);
    camera.reset(new Camera(aspectRatio));

    lastFrame = static_cast<float>(glfwGetTime());

    glfwSetWindowUserPointer(glfwWindow, camera.get());
    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetFramebufferSizeCallback(glfwWindow, framebufferSizeCallback);
    glfwSetCursorPosCallback(glfwWindow, Camera::rotate);
    glfwSetScrollCallback(glfwWindow, Camera::zoom);

    glEnable(GL_DEPTH_TEST);

    return true;
}

void Window::update(GLuint shaderProgram)
{
    if (shaderProgram == 0) {
        close();
        return;
    }

    const GLint modelUniform = glGetUniformLocation(shaderProgram, "model");
    const GLint viewUniform = glGetUniformLocation(shaderProgram, "view");
    const GLint projectionUniform = glGetUniformLocation(shaderProgram, "projection");
    const GLint colorUniform = glGetUniformLocation(shaderProgram, "objectColor");

    const float currentFrame = static_cast<float>(glfwGetTime());
    const float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(glfwWindow, GLFW_TRUE);
    }

    camera->update(glfwWindow, deltaTime); // This will be removed when the camere is dependant of scene's spaceship

    glClearColor(0.02f, 0.02f, 0.08f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    const glm::mat4 view = camera->getViewMatrix();
    const glm::mat4 projection = camera->getProjectionMatrix();

    glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));

    scene.update(modelUniform, colorUniform, currentFrame);

    glfwSwapBuffers(glfwWindow);
    glfwPollEvents();
}

void Window::close()
{
    glfwDestroyWindow(glfwWindow);
    glfwWindow = nullptr;
}

GLFWwindow *Window::getGlfwWindow() const { return glfwWindow; }
