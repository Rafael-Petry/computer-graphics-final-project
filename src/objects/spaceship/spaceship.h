#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include "../../helpers/collision/colliders/boundingBox.h"
#include "../object.h"

class Window;

class Spaceship : public Object
{
public:
    Spaceship(const Spaceship &) = delete;
    Spaceship &operator=(const Spaceship &) = delete;

    static Spaceship &getInstance();

    void update(GLint modelUniform, GLint colorUniform, Window *window);
    void updateRotation(Window *window);

    glm::vec4 getPosition() const;
    glm::mat4 getViewMatrix() const;
    static void updateView(GLFWwindow *window, double xpos, double ypos);

protected:
    glm::mat4 translate(Window *window) override;
    glm::mat4 rotate(Window *window) override;
    glm::mat4 scale(Window *window) override;

private:
    Spaceship(const glm::vec3 &color = glm::vec3(0.73f, 0.79f, 0.88f));

    void shoot() const;
    void updateOrientation(float deltaYaw, float deltaPitch, float deltaRoll);

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

    float rotationAcceleration = 0.2f;
    float maxRotationSpeed = 60.0f;
    float rotationDrag = 0.01f;
    float rotationVelocityEpsilon = 0.01f;

    float movementAcceleration = 8.0f;
    float maxMovementSpeed = 5.0f;
    float movementDrag = 0.2f;
    float movementVelocityEpsilon = 0.005f;

    float mouseSensitivity = 0.12f;
    bool firstMouseUpdate = true;
    float lastMouseX = 0.0f;
    float lastMouseY = 0.0f;

    bool isRolling = false;

    static Mesh mesh;
    static BoundingBox boundingBox;
};

#endif
