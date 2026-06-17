#include <iostream>
#include <cmath>
#include <string>
#include <utility>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/geometric.hpp>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "../scene/scene.h"
#include "../objects/celestialBody/sun/sun.h"
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
    Window &window = Window::getInstance();

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(glfwWindow, GLFW_TRUE);
    }

    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        Spaceship::getInstance().toggleCameraMode();
    }

    if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
        window.isFullscreen = !window.isFullscreen;
        if (window.isFullscreen) {
            GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
            const GLFWvidmode *mode = glfwGetVideoMode(primaryMonitor);
            glfwSetWindowMonitor(window.getGlfwWindow(), primaryMonitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        } else {
            glfwSetWindowMonitor(window.getGlfwWindow(), NULL, 0, 0, window.width, window.height, GLFW_DONT_CARE);
        }
    }
}

bool Window::initialize(int width, int height, std::string title)
{
    this->title = std::move(title);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    this->width = width;
    this->height = height;
    glfwWindow = glfwCreateWindow(width, height, this->title.c_str(), nullptr, nullptr);
    if (this->isFullscreen) {
        GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(primaryMonitor);
        glfwSetWindowMonitor(glfwWindow, primaryMonitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }

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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    imguiInitialized = true;

    return true;
}

void Window::update()
{
    if (shaderProgram == 0) {
        close();
        return;
    }

    if (imguiInitialized) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    updateScene();
    updateShaderProgram();
    updateTime();

    if (imguiInitialized) {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    glfwSetKeyCallback(glfwWindow, keyCallback);
    glfwSwapBuffers(glfwWindow);
    glfwPollEvents();
}

void Window::updateShaderProgram()
{
    // ── Matrices view / projection ───────────────────────────────────────────
    const GLint viewUniform = glGetUniformLocation(shaderProgram, "view");
    const GLint projectionUniform = glGetUniformLocation(shaderProgram, "projection");

    const glm::mat4 projection = Matrix_Perspective(static_cast<float>(M_PI) / 3.0f, aspectRatio, -0.1f, -1000.0f);
    const glm::mat4 view = Spaceship::getInstance().getViewMatrix();

    glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));
}

void Window::updateTime()
{
    currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void Window::updateScene()
{
    glClearColor(0.02f, 0.02f, 0.08f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const GLint modelUniform = glGetUniformLocation(shaderProgram, "model");
    const GLint colorUniform = glGetUniformLocation(shaderProgram, "objectColor");
    const GLint useTextureUniform = glGetUniformLocation(shaderProgram, "useTexture");
    const GLint texSamplerUniform = glGetUniformLocation(shaderProgram, "textureColormap");
    const GLint isEmissiveUniform = glGetUniformLocation(shaderProgram, "isEmissive");
    const GLint metallicUniform = glGetUniformLocation(shaderProgram, "metallic");
    const GLint roughnessUniform = glGetUniformLocation(shaderProgram, "roughness");
    const GLint specularUniform = glGetUniformLocation(shaderProgram, "specular");
    scene->update(modelUniform, colorUniform, useTextureUniform, texSamplerUniform, isEmissiveUniform, metallicUniform, roughnessUniform, specularUniform, this);
}

void Window::close()
{
    if (imguiInitialized) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        imguiInitialized = false;
    }

    glfwDestroyWindow(glfwWindow);
    glfwWindow = nullptr;
}

GLFWwindow *Window::getGlfwWindow() const { return glfwWindow; }
float Window::getDeltaTime() const { return deltaTime; }
float Window::getCurrentFrame() const { return currentFrame; }

void Window::setShaderProgram(GLuint shaderProgram)
{
    this->shaderProgram = shaderProgram;
    const glm::vec3 sunPos = Sun::getInstance().getPosition();
    glUniform3f(glGetUniformLocation(shaderProgram, "sunPosition"), sunPos.x, sunPos.y, sunPos.z);
}