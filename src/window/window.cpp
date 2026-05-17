#include <iostream>
#include <cmath>
#include <string>
#include <utility>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../scene/scene.h"
#include "window.h"
#include "../../vendor/include/matrices.h"

Window &Window::getInstance()
{
    static Window instance;
    return instance;
}

void Window::framebufferSizeCallback(GLFWwindow *glfwWindow, int width, int height)
{
    glViewport(0, 0, width, height);

    if (height <= 0) {
        return;
    }

    Window::getInstance().aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

void Window::keyCallback(GLFWwindow *glfwWindow, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(glfwWindow, GLFW_TRUE);
    }

    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        Window &window = Window::getInstance();
        window.setUseSceneCamera(!window.useSceneCamera);
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

    scene.reset(new Scene());

    aspectRatio = static_cast<float>(framebufferWidth) / static_cast<float>(framebufferHeight);
    lastFrame = static_cast<float>(glfwGetTime());

    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetFramebufferSizeCallback(glfwWindow, framebufferSizeCallback);
    glfwSetCursorPosCallback(glfwWindow, &Spaceship::updateView);

    glEnable(GL_DEPTH_TEST);

    return true;
}

void Window::update(GLuint shaderProgram)
{
    if (shaderProgram == 0) {
        close();
        return;
    }

    updateShaderProgram(shaderProgram);
    updateTime();
    updateScene(shaderProgram);

    glfwSetKeyCallback(glfwWindow, keyCallback);
    glfwSwapBuffers(glfwWindow);
    glfwPollEvents();
}

void Window::updateShaderProgram(GLuint shaderProgram)
{
    const GLint viewUniform = glGetUniformLocation(shaderProgram, "view");
    const GLint projectionUniform = glGetUniformLocation(shaderProgram, "projection");

    const glm::mat4 projection = Matrix_Perspective(M_PI / 3.0f, aspectRatio, -0.1f, -100.0f);

    glm::mat4 view = Spaceship::getInstance().getViewMatrix();
    if (useSceneCamera) {
        view = Matrix_cameraView(glm::vec4(0.0, 0.0, 10.0, 1.0f), glm::vec4(0.0, 0.0, -1.0, 0.0f), glm::vec4(0.0, 1.0, 0.0, 0.0f));
    }

    glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));

    glUseProgram(shaderProgram);
}

void Window::updateTime()
{
    currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void Window::updateScene(GLuint shaderProgram)
{
    glClearColor(0.02f, 0.02f, 0.08f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const GLint modelUniform = glGetUniformLocation(shaderProgram, "model");
    const GLint colorUniform = glGetUniformLocation(shaderProgram, "objectColor");
    scene->update(modelUniform, colorUniform, this);
}

void Window::close()
{
    glfwDestroyWindow(glfwWindow);
    glfwWindow = nullptr;
}

GLFWwindow *Window::getGlfwWindow() const { return glfwWindow; }
float Window::getDeltaTime() const { return deltaTime; }
float Window::getCurrentFrame() const { return currentFrame; }
