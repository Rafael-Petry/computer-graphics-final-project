#ifndef CAMERA_H
#define CAMERA_H

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera
{
public:
    Camera(float initialAspectRatio);

    void updateAspectRatio(int width, int height);
    void update(GLFWwindow *window, float deltaTime);
    void rotate(double xpos, double ypos);
    void processScroll(double yoffset);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    static void rotate(GLFWwindow *window, double xpos, double ypos);
    static void zoom(GLFWwindow *window, double xoffset, double yoffset);

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    float mouseSensitivity;
    float movementSpeed;
    float zoomRadians;

    float aspectRatio;

    bool firstMouseUpdate;
    float lastMouseX;
    float lastMouseY;

    void updateOrientation();
};

#endif
