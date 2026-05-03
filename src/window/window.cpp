#include "window.h"

Window &Window::getInstance()
{
    static Window instance;
    return instance;
}

void Window::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);

    Camera *camera = static_cast<Camera *>(glfwGetWindowUserPointer(window));
    if (camera != nullptr) {
        camera->onResize(width, height);
    }
}

void Window::setupCallbacks()
{
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, Camera::cursorPosCallback);
    glfwSetScrollCallback(window, Camera::scrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::initialize(int width, int height, std::string title)
{
    this->width = width;
    this->height = height;
    this->title = std::move(title);
    glfwInitialized = true;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(width, height, this->title.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window." << std::endl;
        close();
        return;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD." << std::endl;
        close();
        return;
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

    return;
}

void Window::close()
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

    float lastFrame = static_cast<float>(glfwGetTime());

    Scene scene = Scene();

    while (!glfwWindowShouldClose(window)) {
        const float currentFrame = static_cast<float>(glfwGetTime());
        const float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        InputHelper::getWindowInputs(window);
        camera->update(window, deltaTime);

        glClearColor(0.02f, 0.02f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        const glm::mat4 view = camera->getViewMatrix();
        const glm::mat4 projection = camera->getProjectionMatrix();

        glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));

        scene.update(modelUniform, colorUniform, currentFrame);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    close();

    return;
}

GLFWwindow *Window::getWindow() const { return window; }
