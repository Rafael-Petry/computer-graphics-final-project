#include <cmath>
#include <iostream>
#include <string>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "spaceship.h"
#include "../../helpers/movement/movement.h"
#include "../../window/window.h"
#include "../../vendor/include/matrices.h"

Spaceship::Spaceship(const std::string &meshPath, const glm::vec3 &color) : Object(meshPath, color)
{
    front = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    right = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
    worldUp = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    position = glm::vec4(0.0f, 0.2f, 5.0f, 1.0f);
    velocity = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
}

Spaceship &Spaceship::getInstance()
{
    static Spaceship instance;
    return instance;
}

void Spaceship::update(GLint modelUniform, GLint colorUniform, Window *window)
{
    Object::update(modelUniform, colorUniform, window);

    isRolling = false;
    if (glfwGetKey(window->getGlfwWindow(), GLFW_KEY_R) == GLFW_PRESS) {
        isRolling = true;
    }

    const float deltaTime = window->getDeltaTime();
    const float rotationDamping = expf(-rotationDrag * deltaTime);

    yawVelocity *= rotationDamping;
    pitchVelocity *= rotationDamping;
    rollVelocity *= rotationDamping;

    if (fabsf(yawVelocity) < rotationVelocityEpsilon) {
        yawVelocity = 0.0f;
    }
    if (fabsf(pitchVelocity) < rotationVelocityEpsilon) {
        pitchVelocity = 0.0f;
    }
    if (fabsf(rollVelocity) < rotationVelocityEpsilon) {
        rollVelocity = 0.0f;
    }

    if (yawVelocity > maxRotationSpeed) {
        yawVelocity = maxRotationSpeed;
    }
    if (yawVelocity < -maxRotationSpeed) {
        yawVelocity = -maxRotationSpeed;
    }

    if (pitchVelocity > maxRotationSpeed) {
        pitchVelocity = maxRotationSpeed;
    }
    if (pitchVelocity < -maxRotationSpeed) {
        pitchVelocity = -maxRotationSpeed;
    }

    if (rollVelocity > maxRotationSpeed) {
        rollVelocity = maxRotationSpeed;
    }
    if (rollVelocity < -maxRotationSpeed) {
        rollVelocity = -maxRotationSpeed;
    }

    float deltaYaw = yawVelocity * deltaTime;
    float deltaPitch = pitchVelocity * deltaTime;
    float deltaRoll = rollVelocity * deltaTime;

    float targetPitch = pitch + deltaPitch;
    if (targetPitch > 89.0f) {
        deltaPitch = 89.0f - pitch;
        pitch = 89.0f;
        pitchVelocity = 0.0f;
    }
    else if (targetPitch < -89.0f) {
        deltaPitch = -89.0f - pitch;
        pitch = -89.0f;
        pitchVelocity = 0.0f;
    }
    else {
        pitch = targetPitch;
    }

    yaw += deltaYaw;
    roll += deltaRoll;

    updateOrientation(deltaYaw, deltaPitch, deltaRoll);
}

glm::vec4 Spaceship::getPosition() const { return position; }

void Spaceship::updateView(GLFWwindow *window, double xpos, double ypos)
{
    Spaceship *spaceship = &Spaceship::getInstance();

    if (spaceship != nullptr) {
        spaceship->isRolling = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;

        if (spaceship->firstMouseUpdate) {
            spaceship->lastMouseX = static_cast<float>(xpos);
            spaceship->lastMouseY = static_cast<float>(ypos);
            spaceship->firstMouseUpdate = false;
        }

        float xoffset = spaceship->lastMouseX - static_cast<float>(xpos);
        float yoffset = spaceship->lastMouseY - static_cast<float>(ypos);

        spaceship->lastMouseX = static_cast<float>(xpos);
        spaceship->lastMouseY = static_cast<float>(ypos);

        xoffset *= spaceship->mouseSensitivity;
        yoffset *= spaceship->mouseSensitivity;

        if (spaceship->isRolling) {
            spaceship->rollVelocity += xoffset * spaceship->rotationAcceleration;
        }
        else {
            spaceship->yawVelocity += xoffset * spaceship->rotationAcceleration;
            spaceship->pitchVelocity += yoffset * spaceship->rotationAcceleration;
        }
    }
}

void Spaceship::shoot() const { std::cout << "The spaceship is shooting..." << std::endl; }

void Spaceship::updateOrientation(float deltaYaw, float deltaPitch, float deltaRoll)
{
    if (deltaYaw != 0.0f) {
        const float yawRad = deltaYaw * (M_PI / 180.0f);
        const glm::mat4 yawMatrix = Matrix_Rotate(yawRad, up);
        front = yawMatrix * front;
        right = yawMatrix * right;
    }

    if (deltaPitch != 0.0f) {
        const float pitchRad = deltaPitch * (M_PI / 180.0f);
        const glm::mat4 pitchMatrix = Matrix_Rotate(pitchRad, right);
        front = pitchMatrix * front;
        up = pitchMatrix * up;
    }

    if (deltaRoll != 0.0f) {
        const float rollRad = deltaRoll * (M_PI / 180.0f);
        const glm::mat4 rollMatrix = Matrix_Rotate(rollRad, front);
        right = rollMatrix * right;
        up = rollMatrix * up;
    }

    front = front / norm(front);
    right = crossproduct(front, up);
    right = right / norm(right);
    up = crossproduct(right, front);
    up = up / norm(up);
}

glm::mat4 Spaceship::translate(Window *window)
{
    const float deltaTime = window->getDeltaTime();
    const glm::vec3 movement = MovementHelper::getMovementInputs(window->getGlfwWindow());

    glm::vec4 acceleration = (front * movement.z) + (right * movement.x) + (up * movement.y);
    acceleration *= movementAcceleration;
    acceleration.w = 0.0f;

    velocity += acceleration * deltaTime;

    const float movementDamping = expf(-movementDrag * deltaTime);
    velocity *= movementDamping;

    if (norm(velocity) < movementVelocityEpsilon) {
        velocity = glm::vec4(0.0f);
    }

    const float speed = norm(velocity);
    if (speed > maxMovementSpeed) {
        velocity = (velocity / speed) * maxMovementSpeed;
    }

    position += velocity * deltaTime;

    return Matrix_Translate(position.x, position.y, position.z);
}

glm::mat4 Spaceship::rotate(Window *window)
{
    const glm::vec3 forward = -front;
    return Matrix(right.x, up.x, forward.x, 0.0f, right.y, up.y, forward.y, 0.0f, right.z, up.z, forward.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

glm::mat4 Spaceship::scale(Window *window) { return Matrix_Scale(0.3f, 0.3f, 0.3f); }

glm::mat4 Spaceship::getViewMatrix() const { return Matrix_cameraView(position + front, front, up); }
