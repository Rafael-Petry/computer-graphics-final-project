#include "camera.h"

#include <cmath>

#include <glm/geometric.hpp>

#include "../helpers/input/input.h"

glm::mat4 Matrix_cameraView(glm::vec4 position_c, glm::vec4 view_vector, glm::vec4 up_vector);
glm::mat4 Matrix_Perspective(float field_of_view, float aspect, float n, float f);

Camera::Camera(float initialAspectRatio)
    : position(0.0f, 1.0f, 7.0f), front(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f), right(1.0f, 0.0f, 0.0f), worldUp(0.0f, 1.0f, 0.0f), yaw(-90.0f), pitch(0.0f), mouseSensitivity(0.12f),
      movementSpeed(5.0f), zoomRadians(3.1415926f / 3.0f), aspectRatio(initialAspectRatio), firstMouseUpdate(true), lastMouseX(0.0f), lastMouseY(0.0f)
{
    updateDirectionVectors();
}

void Camera::onResize(int width, int height)
{
    if (height <= 0) {
        return;
    }

    aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

void Camera::update(GLFWwindow *window, float deltaTime)
{
    const float velocity = movementSpeed * deltaTime;
    const glm::vec3 movement = InputHelper::getMovementInputs(window);

    position += front * (movement.z * velocity);
    position += right * (movement.x * velocity);
    position += worldUp * (movement.y * velocity);
}

void Camera::processMouse(double xpos, double ypos)
{
    if (firstMouseUpdate) {
        lastMouseX = static_cast<float>(xpos);
        lastMouseY = static_cast<float>(ypos);
        firstMouseUpdate = false;
    }

    float xoffset = static_cast<float>(xpos) - lastMouseX;
    float yoffset = lastMouseY - static_cast<float>(ypos);

    lastMouseX = static_cast<float>(xpos);
    lastMouseY = static_cast<float>(ypos);

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    updateDirectionVectors();
}

void Camera::processScroll(double yoffset)
{
    zoomRadians -= static_cast<float>(yoffset) * 0.03f;

    const float minZoom = 3.1415926f / 12.0f;
    const float maxZoom = 3.1415926f / 1.5f;

    if (zoomRadians < minZoom) {
        zoomRadians = minZoom;
    }

    if (zoomRadians > maxZoom) {
        zoomRadians = maxZoom;
    }
}

glm::mat4 Camera::getViewMatrix() const { return Matrix_cameraView(glm::vec4(position, 1.0f), glm::vec4(front, 0.0f), glm::vec4(up, 0.0f)); }

glm::mat4 Camera::getProjectionMatrix() const { return Matrix_Perspective(zoomRadians, aspectRatio, -0.1f, -100.0f); }

void Camera::cursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
    Camera *camera = static_cast<Camera *>(glfwGetWindowUserPointer(window));
    if (camera != nullptr) {
        camera->processMouse(xpos, ypos);
    }
}

void Camera::scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    (void)xoffset;

    Camera *camera = static_cast<Camera *>(glfwGetWindowUserPointer(window));
    if (camera != nullptr) {
        camera->zoomRadians -= static_cast<float>(yoffset) * 0.03f;

        const float minZoom = 3.1415926f / 12.0f;
        const float maxZoom = 3.1415926f / 1.5f;

        if (camera->zoomRadians < minZoom) {
            camera->zoomRadians = minZoom;
        }
        if (camera->zoomRadians > maxZoom) {
            camera->zoomRadians = maxZoom;
        }
    }
}

void Camera::updateDirectionVectors()
{
    const float yawRad = yaw * (3.1415926f / 180.0f);
    const float pitchRad = pitch * (3.1415926f / 180.0f);

    glm::vec3 newFront;
    newFront.x = cosf(yawRad) * cosf(pitchRad);
    newFront.y = sinf(pitchRad);
    newFront.z = sinf(yawRad) * cosf(pitchRad);

    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
