#include "window.h"

#include <cmath>
#include <iostream>
#include <utility>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include "../camera/camera.h"
#include "../scene/scene.h"
#include "../helpers/input/input.h"
#include "../shaders/loader/loader.h"

Window::Window(int width, int height, std::string title) : width(width), height(height), title(std::move(title)), window(nullptr), camera(nullptr), glfwInitialized(false) { initialize(); }

Window::~Window() { cleanup(); }

bool Window::initialize()
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return false;
    }

    glfwInitialized = true;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window." << std::endl;
        cleanup();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD." << std::endl;
        cleanup();
        return false;
    }

    int framebufferWidth = 0;
    int framebufferHeight = 0;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);

    const float aspectRatio = static_cast<float>(framebufferWidth) / static_cast<float>(framebufferHeight);
    camera.reset(new Camera(aspectRatio));

    glfwSetWindowUserPointer(window, camera.get());
    setupCallbacks();
    glEnable(GL_DEPTH_TEST);

    return true;
}

void Window::setupCallbacks()
{
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetCursorPosCallback(window, Camera::CursorPosCallback);
    glfwSetScrollCallback(window, Camera::ScrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::cleanup()
{
    if (window != nullptr) {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    if (glfwInitialized) {
        glfwTerminate();
        glfwInitialized = false;
    }
}

void Window::FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);

    Camera *camera = static_cast<Camera *>(glfwGetWindowUserPointer(window));
    if (camera != nullptr) {
        camera->onResize(width, height);
    }
}

int Window::run()
{
    const GLuint shaderProgram = ShaderLoader::CreateShaderProgram("../../src/shaders/vertex.glsl", "../../src/shaders/fragment.glsl");

    if (shaderProgram == 0) {
        cleanup();
        return -1;
    }

    const GLint modelUniform = glGetUniformLocation(shaderProgram, "model");
    const GLint viewUniform = glGetUniformLocation(shaderProgram, "view");
    const GLint projectionUniform = glGetUniformLocation(shaderProgram, "projection");
    const GLint colorUniform = glGetUniformLocation(shaderProgram, "objectColor");

    float lastFrame = static_cast<float>(glfwGetTime());

    Scene scene = Scene();

    while (!glfwWindowShouldClose(window)) {
        const float currentFrame = static_cast<float>(glfwGetTime());
        const float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        InputHelper::ListenWindowInputs(window);
        camera->processKeyboard(window, deltaTime);

        glClearColor(0.02f, 0.02f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        const glm::mat4 view = camera->getViewMatrix();
        const glm::mat4 projection = camera->getProjectionMatrix();

        glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));

        scene.render(modelUniform, colorUniform, currentFrame);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    cleanup();

    return 0;
}
