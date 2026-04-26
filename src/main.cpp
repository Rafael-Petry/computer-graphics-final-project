#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include "camera/camera.h"
#include "celestialBody/asteroid/asteroid.h"
#include "celestialBody/planet/planet.h"
#include "celestialBody/sun/sun.h"
#include "spaceship/spaceship.h"
#include "vendor/include/matrices.h"

namespace
{
Camera* g_camera = nullptr;

std::string ReadTextFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << path << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint CompileShader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

        std::string log(static_cast<size_t>(logLength), '\0');
        glGetShaderInfoLog(shader, logLength, nullptr, &log[0]);
        std::cerr << "Shader compilation error: " << log << std::endl;
    }

    return shader;
}

GLuint CreateShaderProgram()
{
    const std::string vertexShaderSource = ReadTextFile("../../src/shaders/vertex.glsl");
    const std::string fragmentShaderSource = ReadTextFile("../../src/shaders/fragment.glsl");

    if (vertexShaderSource.empty() || fragmentShaderSource.empty())
    {
        return 0;
    }

    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource.c_str());
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource.c_str());

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLint logLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

        std::string log(static_cast<size_t>(logLength), '\0');
        glGetProgramInfoLog(program, logLength, nullptr, &log[0]);
        std::cerr << "Shader link error: " << log << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);

    if (g_camera != nullptr)
    {
        g_camera->onResize(width, height);
    }
}

void ProcessCommonInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}
} // namespace

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Computer Graphics Final Project", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    int framebufferWidth = 0;
    int framebufferHeight = 0;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);

    Camera camera(static_cast<float>(framebufferWidth) / static_cast<float>(framebufferHeight));
    g_camera = &camera;

    glfwSetWindowUserPointer(window, &camera);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetCursorPosCallback(window, Camera::CursorPosCallback);
    glfwSetScrollCallback(window, Camera::ScrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    const GLuint shaderProgram = CreateShaderProgram();
    if (shaderProgram == 0)
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    const GLint modelUniform = glGetUniformLocation(shaderProgram, "model");
    const GLint viewUniform = glGetUniformLocation(shaderProgram, "view");
    const GLint projectionUniform = glGetUniformLocation(shaderProgram, "projection");
    const GLint colorUniform = glGetUniformLocation(shaderProgram, "objectColor");

    Spaceship spaceship;
    Sun& sun = Sun::getInstance();
    Asteroid asteroid;
    Planet planetA;
    Planet planetB;
    Planet planetC;

    float lastFrame = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(window))
    {
        const float currentFrame = static_cast<float>(glfwGetTime());
        const float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        ProcessCommonInput(window);
        camera.processKeyboard(window, deltaTime);

        glClearColor(0.02f, 0.02f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        const glm::mat4 view = camera.getViewMatrix();
        const glm::mat4 projection = camera.getProjectionMatrix();

        glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));

        const glm::mat4 sunModel = Matrix_Scale(1.1f, 1.1f, 1.1f);
        sun.render(modelUniform, colorUniform, sunModel);

        const glm::mat4 spaceshipModel =
            Matrix_Translate(0.0f, 0.2f, 5.0f)
            * Matrix_Rotate_Y(currentFrame)
            * Matrix_Scale(0.4f, 0.25f, 1.0f);
        spaceship.render(modelUniform, colorUniform, spaceshipModel);

        const glm::mat4 asteroidModel =
            Matrix_Translate(2.7f * cosf(currentFrame * 0.8f), 0.4f, 2.7f * sinf(currentFrame * 0.8f))
            * Matrix_Rotate_Y(currentFrame * 1.7f)
            * Matrix_Scale(0.35f, 0.35f, 0.35f);
        asteroid.render(modelUniform, colorUniform, asteroidModel);

        const glm::mat4 planetAModel =
            Matrix_Translate(2.0f * cosf(currentFrame * 0.4f), 0.0f, 2.0f * sinf(currentFrame * 0.4f))
            * Matrix_Scale(0.45f, 0.45f, 0.45f);
        planetA.render(modelUniform, colorUniform, planetAModel);

        const glm::mat4 planetBModel =
            Matrix_Translate(3.3f * cosf(currentFrame * 0.25f), -0.1f, 3.3f * sinf(currentFrame * 0.25f))
            * Matrix_Scale(0.6f, 0.6f, 0.6f);
        planetB.render(modelUniform, colorUniform, planetBModel);

        const glm::mat4 planetCModel =
            Matrix_Translate(4.6f * cosf(currentFrame * 0.18f), 0.15f, 4.6f * sinf(currentFrame * 0.18f))
            * Matrix_Scale(0.35f, 0.35f, 0.35f);
        planetC.render(modelUniform, colorUniform, planetCModel);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}