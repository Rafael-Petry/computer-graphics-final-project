#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

class Scene;

class Window
{
public:
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    static Window &getInstance();

    bool initialize(int width, int height, std::string title);
    void update();
    void close();

    GLFWwindow *getGlfwWindow() const;
    float getDeltaTime() const;
    float getCurrentFrame() const;
    void setShaderProgram(GLuint shaderProgram);

private:
    static void framebufferSizeCallback(GLFWwindow *glfwWindow, int width, int height);
    static void keyCallback(GLFWwindow *glfwWindow, int key, int scancode, int action, int mods);
    void updateShaderProgram();
    void updateTime();
    void updateScene();

    std::string title;
    GLFWwindow *glfwWindow = nullptr;
    std::unique_ptr<Scene> scene;
    bool glfwInitialized = false;
    float currentFrame = 0.0f;
    float lastFrame = 0.0f;
    float deltaTime = 0.0f;

    bool imguiInitialized = false;

    float aspectRatio;

    bool isFullscreen = true;

    int width = 1280;
    int height = 720;

    GLuint shaderProgram;

    Window() = default;
};

#endif
