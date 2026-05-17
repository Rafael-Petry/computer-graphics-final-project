#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <string>

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include "../object.h"

class Window;

class Spaceship : public Object
{
public:
    Spaceship(const std::string &meshPath = "../../src/objects/spaceship/spaceship.obj", const glm::vec3 &color = glm::vec3(0.73f, 0.79f, 0.88f));

    void update(GLint modelUniform, GLint colorUniform, Window *window);

    glm::mat4 getViewMatrix() const;
    static void updateView(GLFWwindow *window, double xpos, double ypos);

protected:
    glm::mat4 translate(Window *window) override;
    glm::mat4 rotate(Window *window) override;
    glm::mat4 scale(Window *window) override;

private:
    void shoot() const;
    void updateOrientation();

    glm::vec4 position;
    glm::vec4 velocity;
    glm::vec4 front;
    glm::vec4 up;
    glm::vec4 right;
    glm::vec4 worldUp;

    float yaw = -90.0f;
    float pitch = 0.0f;
    float roll = 0.0f;

    float yawVelocity = 0.0f;
    float pitchVelocity = 0.0f;
    float rollVelocity = 0.0f;

    float rotationAcceleration = 8.0f;
    float maxRotationSpeed = 120.0f;

    float movementAcceleration = 8.0f;
    float maxMovementSpeed = 5.0f;

    float mouseSensitivity = 0.12f;
    bool firstMouseUpdate = true;
    float lastMouseX = 0.0f;
    float lastMouseY = 0.0f;

    bool isRolling = false;
};

#endif
