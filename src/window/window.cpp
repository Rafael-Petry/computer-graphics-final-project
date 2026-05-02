#include "window.h"

#include <cmath>
#include <iostream>
#include <utility>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include "camera/camera.h"
#include "celestialBody/asteroid/asteroid.h"
#include "celestialBody/planet/planet.h"
#include "celestialBody/sun/sun.h"
#include "helpers/input/input.h"
#include "shaders/loader/loader.h"
#include "spaceship/spaceship.h"
#include "vendor/include/matrices.h"

Window::Window(int width, int height, std::string title)
	: width_(width),
	  height_(height),
	  title_(std::move(title)),
	  window_(nullptr),
	  camera_(nullptr),
	  glfwInitialized_(false)
{
}

Window::~Window()
{
	cleanup();
}

bool Window::initialize()
{
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW." << std::endl;
		return false;
	}
	glfwInitialized_ = true;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	window_ = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);
	if (window_ == nullptr)
	{
		std::cerr << "Failed to create GLFW window." << std::endl;
		cleanup();
		return false;
	}

	glfwMakeContextCurrent(window_);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD." << std::endl;
		cleanup();
		return false;
	}

	int framebufferWidth = 0;
	int framebufferHeight = 0;
	glfwGetFramebufferSize(window_, &framebufferWidth, &framebufferHeight);
	glViewport(0, 0, framebufferWidth, framebufferHeight);

	const float aspectRatio = static_cast<float>(framebufferWidth) / static_cast<float>(framebufferHeight);
	camera_ = std::make_unique<Camera>(aspectRatio);

	glfwSetWindowUserPointer(window_, camera_.get());
	setupCallbacks();
	glEnable(GL_DEPTH_TEST);

	return true;
}

void Window::setupCallbacks()
{
	glfwSetFramebufferSizeCallback(window_, FramebufferSizeCallback);
	glfwSetCursorPosCallback(window_, Camera::CursorPosCallback);
	glfwSetScrollCallback(window_, Camera::ScrollCallback);
	glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::cleanup()
{
	if (window_ != nullptr)
	{
		glfwDestroyWindow(window_);
		window_ = nullptr;
	}

	if (glfwInitialized_)
	{
		glfwTerminate();
		glfwInitialized_ = false;
	}
}

void Window::FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);

	Camera *camera = static_cast<Camera *>(glfwGetWindowUserPointer(window));
	if (camera != nullptr)
	{
		camera->onResize(width, height);
	}
}

int Window::run()
{
	if (!initialize())
	{
		return -1;
	}

	const GLuint shaderProgram = ShaderLoader::CreateShaderProgram(
		"../../src/shaders/vertex.glsl",
		"../../src/shaders/fragment.glsl");
	if (shaderProgram == 0)
	{
		cleanup();
		return -1;
	}

	const GLint modelUniform = glGetUniformLocation(shaderProgram, "model");
	const GLint viewUniform = glGetUniformLocation(shaderProgram, "view");
	const GLint projectionUniform = glGetUniformLocation(shaderProgram, "projection");
	const GLint colorUniform = glGetUniformLocation(shaderProgram, "objectColor");

	Spaceship spaceship;
	Sun &sun = Sun::getInstance();
	Asteroid asteroid;
	Planet planetA;
	Planet planetB;
	Planet planetC;

	float lastFrame = static_cast<float>(glfwGetTime());

	while (!glfwWindowShouldClose(window_))
	{
		const float currentFrame = static_cast<float>(glfwGetTime());
		const float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		InputHelper::ProcessCommonInput(window_);
		camera_->processKeyboard(window_, deltaTime);

		glClearColor(0.02f, 0.02f, 0.08f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);

		const glm::mat4 view = camera_->getViewMatrix();
		const glm::mat4 projection = camera_->getProjectionMatrix();

		glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));

		const glm::mat4 sunModel = Matrix_Scale(1.1f, 1.1f, 1.1f);
		sun.render(modelUniform, colorUniform, sunModel);

		const glm::mat4 spaceshipModel =
			Matrix_Translate(0.0f, 0.2f, 5.0f) * Matrix_Rotate_Y(currentFrame) * Matrix_Scale(0.4f, 0.25f, 1.0f);
		spaceship.render(modelUniform, colorUniform, spaceshipModel);

		const glm::mat4 asteroidModel =
			Matrix_Translate(2.7f * cosf(currentFrame * 0.8f), 0.4f, 2.7f * sinf(currentFrame * 0.8f)) * Matrix_Rotate_Y(currentFrame * 1.7f) * Matrix_Scale(0.35f, 0.35f, 0.35f);
		asteroid.render(modelUniform, colorUniform, asteroidModel);

		const glm::mat4 planetAModel =
			Matrix_Translate(2.0f * cosf(currentFrame * 0.4f), 0.0f, 2.0f * sinf(currentFrame * 0.4f)) * Matrix_Scale(0.45f, 0.45f, 0.45f);
		planetA.render(modelUniform, colorUniform, planetAModel);

		const glm::mat4 planetBModel =
			Matrix_Translate(3.3f * cosf(currentFrame * 0.25f), -0.1f, 3.3f * sinf(currentFrame * 0.25f)) * Matrix_Scale(0.6f, 0.6f, 0.6f);
		planetB.render(modelUniform, colorUniform, planetBModel);

		const glm::mat4 planetCModel =
			Matrix_Translate(4.6f * cosf(currentFrame * 0.18f), 0.15f, 4.6f * sinf(currentFrame * 0.18f)) * Matrix_Scale(0.35f, 0.35f, 0.35f);
		planetC.render(modelUniform, colorUniform, planetCModel);

		glfwSwapBuffers(window_);
		glfwPollEvents();
	}

	glDeleteProgram(shaderProgram);
	cleanup();

	return 0;
}
