#ifndef cameraH
#define cameraH

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Camera
{
public:
    Camera(float initialAspectRatio);

    void onResize(int width, int height);
    void processKeyboard(GLFWwindow *window, float deltaTime);
    void processMouse(double xpos, double ypos);
    void processScroll(double yoffset);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    static void CursorPosCallback(GLFWwindow *window, double xpos, double ypos);
    static void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);

private:
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;

    float m_yaw;
    float m_pitch;

    float m_mouseSensitivity;
    float m_movementSpeed;
    float m_zoomRadians;

    float m_aspectRatio;

    bool m_firstMouseUpdate;
    float m_lastMouseX;
    float m_lastMouseY;

    void updateDirectionVectors();
};

#endif
