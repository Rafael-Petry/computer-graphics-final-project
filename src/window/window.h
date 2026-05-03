// Window class that owns GLFW setup and the render loop.
#pragma once

#include <memory>
#include <string>

struct GLFWwindow;
class Camera;

class Window
{
public:
    Window(int width, int height, std::string title);
    ~Window();

    int run();

private:
    bool initialize();
    void setupCallbacks();
    void cleanup();

    static void FramebufferSizeCallback(GLFWwindow *window, int width, int height);

    int width;
    int height;
    std::string title;
    GLFWwindow *window;
    std::unique_ptr<Camera> camera;
    bool glfwInitialized;
};
