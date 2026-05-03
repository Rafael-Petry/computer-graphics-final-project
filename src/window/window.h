#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include "../camera/camera.h"
#include "../scene/scene.h"

struct GLFWwindow;
class Camera;

class Window
{
public:
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    static Window &getInstance();

    bool initialize(int width, int height, std::string title);
    void update(GLuint shaderProgram);
    void close();

    GLFWwindow *getWindow() const;

private:
    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
    void setupCallbacks();

    int width = 0;
    int height = 0;
    std::string title;
    GLFWwindow *window = nullptr;
    std::unique_ptr<Camera> camera;
    Scene scene;
    bool glfwInitialized = false;
    float lastFrame = 0.0f;

    Window() = default;
};

#endif
