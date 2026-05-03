#ifndef cameraH
#define cameraH

#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Camera
{
public:
    Camera(float initialAspectRatio);

    void onResize(int width, int height);
    void update(GLFWwindow *window, float deltaTime);
    void processMouse(double xpos, double ypos);
    void processScroll(double yoffset);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

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

    void updateDirectionVectors();
};

#endif
