#include <cmath>

#include <GLFW/glfw3.h>

#include <glm/geometric.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "camera.h"
#include "../../vendor/include/matrices.h"

namespace {
    float getMovementAxis(GLFWwindow *window, int positiveKey, int negativeKey)
    {
        const bool positivePressed = glfwGetKey(window, positiveKey) == GLFW_PRESS;
        const bool negativePressed = glfwGetKey(window, negativeKey) == GLFW_PRESS;

        if (positivePressed == negativePressed) {
            return 0.0f;
        }

        return positivePressed ? 1.0f : -1.0f;
    }

    glm::vec3 getMovementInputs(GLFWwindow *window)
    {
        const float forward = getMovementAxis(window, GLFW_KEY_W, GLFW_KEY_S);
        const float right = getMovementAxis(window, GLFW_KEY_D, GLFW_KEY_A);
        const float up = getMovementAxis(window, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_LEFT_CONTROL);

        return glm::vec3(right, up, forward);
    }
} // input namespace

glm::mat4 Matrix_cameraView(glm::vec4 position_c, glm::vec4 view_vector, glm::vec4 up_vector);
glm::mat4 Matrix_Perspective(float field_of_view, float aspect, float n, float f);

Camera::Camera(float initialAspectRatio)
    : position(0.0f, 1.0f, 7.0f), front(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f), right(1.0f, 0.0f, 0.0f), worldUp(0.0f, 1.0f, 0.0f), yaw(-90.0f), pitch(0.0f),
      mouseSensitivity(0.12f), movementSpeed(5.0f), zoomRadians(M_PI / 3.0f), aspectRatio(initialAspectRatio), firstMouseUpdate(true), lastMouseX(0.0f), lastMouseY(0.0f)
{
    updateOrientation();
}

void Camera::updateAspectRatio(int width, int height)
{
    if (height <= 0) {
        return;
    }

    aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

void Camera::updateOrientation()
{
    const float yawRad = yaw * (M_PI / 180.0f);
    const float pitchRad = pitch * (M_PI / 180.0f);

    glm::vec3 newFront;
    newFront.x = cosf(yawRad) * cosf(pitchRad);
    newFront.y = sinf(pitchRad);
    newFront.z = sinf(yawRad) * cosf(pitchRad);

    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::update(GLFWwindow *window, float deltaTime)
{
    const float velocity = movementSpeed * deltaTime;
    const glm::vec3 movement = getMovementInputs(window);

    position += front * (movement.z * velocity);
    position += right * (movement.x * velocity);
    position += worldUp * (movement.y * velocity);
}

void Camera::rotate(GLFWwindow *window, double xpos, double ypos)
{
    Camera *camera = static_cast<Camera *>(glfwGetWindowUserPointer(window));

    if (camera != nullptr) {
        if (camera->firstMouseUpdate) {
            camera->lastMouseX = static_cast<float>(xpos);
            camera->lastMouseY = static_cast<float>(ypos);
            camera->firstMouseUpdate = false;
        }

        float xoffset = static_cast<float>(xpos) - camera->lastMouseX;
        float yoffset = camera->lastMouseY - static_cast<float>(ypos);

        camera->lastMouseX = static_cast<float>(xpos);
        camera->lastMouseY = static_cast<float>(ypos);

        xoffset *= camera->mouseSensitivity;
        yoffset *= camera->mouseSensitivity;

        camera->yaw += xoffset;
        camera->pitch += yoffset;

        if (camera->pitch > 89.0f) {
            camera->pitch = 89.0f;
        }

        if (camera->pitch < -89.0f) {
            camera->pitch = -89.0f;
        }

        camera->updateOrientation();
    }
}

void Camera::zoom(GLFWwindow *window, double xoffset, double yoffset)
{
    (void)xoffset;

    Camera *camera = static_cast<Camera *>(glfwGetWindowUserPointer(window));
    if (camera != nullptr) {
        camera->zoomRadians -= static_cast<float>(yoffset) * 0.03f;

        const float minZoom = M_PI / 12.0f;
        const float maxZoom = M_PI / 1.5f;

        if (camera->zoomRadians < minZoom) {
            camera->zoomRadians = minZoom;
        }
        if (camera->zoomRadians > maxZoom) {
            camera->zoomRadians = maxZoom;
        }
    }
}

glm::mat4 Camera::getViewMatrix() const { return Matrix_cameraView(glm::vec4(position, 1.0f), glm::vec4(front, 0.0f), glm::vec4(up, 0.0f)); }

glm::mat4 Camera::getProjectionMatrix() const { return Matrix_Perspective(zoomRadians, aspectRatio, -0.1f, -100.0f); }
