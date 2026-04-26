#include "camera.h"

#include <cmath>

#include <glm/geometric.hpp>

glm::mat4 Matrix_Camera_View(glm::vec4 position_c, glm::vec4 view_vector, glm::vec4 up_vector);
glm::mat4 Matrix_Perspective(float field_of_view, float aspect, float n, float f);

Camera::Camera(float initialAspectRatio)
    : m_position(0.0f, 1.0f, 7.0f),
      m_front(0.0f, 0.0f, -1.0f),
      m_up(0.0f, 1.0f, 0.0f),
      m_right(1.0f, 0.0f, 0.0f),
      m_worldUp(0.0f, 1.0f, 0.0f),
      m_yaw(-90.0f),
      m_pitch(0.0f),
      m_mouseSensitivity(0.12f),
      m_movementSpeed(5.0f),
      m_zoomRadians(3.1415926f / 3.0f),
      m_aspectRatio(initialAspectRatio),
      m_firstMouseUpdate(true),
      m_lastMouseX(0.0f),
      m_lastMouseY(0.0f)
{
    updateDirectionVectors();
}

void Camera::onResize(int width, int height)
{
    if (height <= 0)
    {
        return;
    }

    m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

void Camera::processKeyboard(GLFWwindow* window, float deltaTime)
{
    const float velocity = m_movementSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_position += m_front * velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_position -= m_front * velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        m_position -= m_right * velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        m_position += m_right * velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        m_position -= m_worldUp * velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        m_position += m_worldUp * velocity;
    }
}

void Camera::processMouse(double xpos, double ypos)
{
    if (m_firstMouseUpdate)
    {
        m_lastMouseX = static_cast<float>(xpos);
        m_lastMouseY = static_cast<float>(ypos);
        m_firstMouseUpdate = false;
    }

    float xoffset = static_cast<float>(xpos) - m_lastMouseX;
    float yoffset = m_lastMouseY - static_cast<float>(ypos);

    m_lastMouseX = static_cast<float>(xpos);
    m_lastMouseY = static_cast<float>(ypos);

    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    if (m_pitch > 89.0f)
    {
        m_pitch = 89.0f;
    }
    if (m_pitch < -89.0f)
    {
        m_pitch = -89.0f;
    }

    updateDirectionVectors();
}

void Camera::processScroll(double yoffset)
{
    m_zoomRadians -= static_cast<float>(yoffset) * 0.03f;

    const float minZoom = 3.1415926f / 12.0f;
    const float maxZoom = 3.1415926f / 1.5f;

    if (m_zoomRadians < minZoom)
    {
        m_zoomRadians = minZoom;
    }
    if (m_zoomRadians > maxZoom)
    {
        m_zoomRadians = maxZoom;
    }
}

glm::mat4 Camera::getViewMatrix() const
{
    return Matrix_Camera_View(
        glm::vec4(m_position, 1.0f),
        glm::vec4(m_front, 0.0f),
        glm::vec4(m_up, 0.0f));
}

glm::mat4 Camera::getProjectionMatrix() const
{
    return Matrix_Perspective(m_zoomRadians, m_aspectRatio, -0.1f, -100.0f);
}

void Camera::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera != nullptr)
    {
        camera->processMouse(xpos, ypos);
    }
}

void Camera::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    (void)xoffset;

    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera != nullptr)
    {
        camera->processScroll(yoffset);
    }
}

void Camera::updateDirectionVectors()
{
    const float yawRad = m_yaw * (3.1415926f / 180.0f);
    const float pitchRad = m_pitch * (3.1415926f / 180.0f);

    glm::vec3 front;
    front.x = cosf(yawRad) * cosf(pitchRad);
    front.y = sinf(pitchRad);
    front.z = sinf(yawRad) * cosf(pitchRad);

    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
