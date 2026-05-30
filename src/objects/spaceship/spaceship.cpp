#include <cmath>
#include <iostream>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>

#include "spaceship.h"
#include "../../helpers/collision/collision.h"
#include "../../helpers/collision/colliders/boundingSphere.h"
#include "../../helpers/render/render.h"
#include "../../helpers/movement/movement.h"
#include "../../window/window.h"
#include "../../objects/celestialBody/asteroid/asteroid.h"
#include "../../vendor/include/matrices.h"

Mesh Spaceship::mesh;
BoundingBox Spaceship::boundingBox;
Mesh Spaceship::crosshairMesh;
Mesh Spaceship::rayMesh;

Spaceship::Spaceship(const glm::vec3 &color) : Object(mesh, boundingBox, color)
{
    if (mesh.vao == 0) {
        mesh = RenderHelper::loadObjMesh("../../src/objects/spaceship/spaceship.obj");
    }

    if (!boundingBox.isInitialized() && mesh.vao != 0) {
        boundingBox = CollisionHelper::generateBoundingBox(mesh);
    }

    if (crosshairMesh.vao == 0) {
        crosshairMesh = RenderHelper::loadObjMesh("../../src/objects/object.obj");
    }

    if (rayMesh.vao == 0) {
        rayMesh = RenderHelper::createCylinderMesh(24);
    }

    front = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    right = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
    worldUp = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    position = glm::vec3(0.0f, 0.2f, 5.0f);
    velocity = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    scaleValue = glm::vec3(0.3f);
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

    updateRotation(window);
}

void Spaceship::updateShooting(GLint modelUniform, GLint colorUniform, Window *window, std::vector<Asteroid> &asteroids)
{
    shoot(window, asteroids);

    if (window->getCurrentFrame() <= rayVisibleUntil) {
        renderRay(modelUniform, colorUniform);
    }

    renderCrosshair(modelUniform, colorUniform);
}

void Spaceship::updateRotation(Window *window)
{
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

    pitch += deltaPitch;
    yaw += deltaYaw;
    roll += deltaRoll;

    updateOrientation(deltaYaw, deltaPitch, deltaRoll);
}

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
            xoffset = -xoffset;
            spaceship->rollVelocity += xoffset * spaceship->rotationAcceleration;
        } else {
            spaceship->yawVelocity += xoffset * spaceship->rotationAcceleration;
            spaceship->pitchVelocity += yoffset * spaceship->rotationAcceleration;
        }
    }
}

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

void Spaceship::shoot(Window *window, std::vector<Asteroid> &asteroids)
{
    const float currentTime = window->getCurrentFrame();
    if (glfwGetMouseButton(window->getGlfwWindow(), GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) {
        return;
    }

    if ((currentTime - lastShotTime) < shotCooldown) {
        return;
    }

    lastShotTime = currentTime;
    rayVisibleUntil = currentTime + rayVisibleDuration;

    rayOrigin = getCameraPosition();
    rayDirection = glm::normalize(glm::vec3(front));

    std::vector<Asteroid *> hitAsteroids;

    for (Asteroid &asteroid : asteroids) {
        const BoundingSphere *sphere = dynamic_cast<const BoundingSphere *>(&asteroid.getCollider());
        if (sphere == nullptr) {
            continue;
        }

        float hitDistance = 0.0f;
        if (sphere->testRay(asteroid, rayOrigin, rayDirection, rayMaxRange, &hitDistance)) {
            hitAsteroids.push_back(&asteroid);
        }
    }

    for (Asteroid *asteroid : hitAsteroids) {
        asteroid->onShotHit();
        addScore(100);
    }
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

    position += glm::vec3(velocity) * deltaTime;

    return Matrix_Translate(position.x, position.y, position.z);
}

glm::mat4 Spaceship::rotate(Window *window) { return Matrix(right.x, up.x, front.x, 0.0f, right.y, up.y, front.y, 0.0f, right.z, up.z, front.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }

glm::mat4 Spaceship::getViewMatrix() const
{
    const glm::vec4 position4(position, 1.0f);
    return Matrix_cameraView(position4 + front, front, up);
}

glm::vec3 Spaceship::getCameraPosition() const { return position + glm::vec3(front); }

const BoundingBox &Spaceship::getBoundingBox() const { return boundingBox; }

int Spaceship::getScore() const { return score; }

int Spaceship::getHealth() const { return health; }

void Spaceship::addScore(int amount)
{
    if (amount <= 0) {
        return;
    }

    score += amount;
}

void Spaceship::applyDamage(int amount)
{
    if (amount <= 0) {
        return;
    }

    health -= amount;
    if (health < 0) {
        health = 0;
    }
}

glm::mat4 Spaceship::getOrientationMatrix() const
{
    return Matrix(right.x, up.x, front.x, 0.0f, right.y, up.y, front.y, 0.0f, right.z, up.z, front.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

void Spaceship::renderCrosshair(GLint modelUniform, GLint colorUniform) const
{
    if (crosshairMesh.vao == 0) {
        return;
    }

    const glm::vec3 center = getCameraPosition() + glm::vec3(front) * crosshairDistance;
    const glm::mat4 orientation = getOrientationMatrix();

    glDisable(GL_DEPTH_TEST);

    const glm::vec3 crosshairColor(0.95f, 0.95f, 0.95f);
    const glm::mat4 base = Matrix_Translate(center.x, center.y, center.z) * orientation;

    const glm::mat4 horizontal = base * Matrix_Scale(crosshairLength, crosshairThickness, crosshairThickness);
    RenderHelper::renderModel(modelUniform, colorUniform, horizontal, crosshairMesh, crosshairColor);

    const glm::mat4 vertical = base * Matrix_Scale(crosshairThickness, crosshairLength, crosshairThickness);
    RenderHelper::renderModel(modelUniform, colorUniform, vertical, crosshairMesh, crosshairColor);

    glEnable(GL_DEPTH_TEST);
}

void Spaceship::renderRay(GLint modelUniform, GLint colorUniform) const
{
    if (rayMesh.vao == 0) {
        return;
    }

    const glm::vec3 direction = glm::normalize(rayDirection);
    glm::vec3 upDir = glm::vec3(worldUp);
    if (std::fabs(glm::dot(direction, upDir)) > 0.98f) {
        upDir = glm::vec3(0.0f, 0.0f, 1.0f);
    }

    const glm::vec3 rightDir = glm::normalize(glm::cross(upDir, direction));
    const glm::vec3 fixedUp = glm::normalize(glm::cross(direction, rightDir));

    const glm::mat4 orientation =
        Matrix(rightDir.x, fixedUp.x, direction.x, 0.0f, rightDir.y, fixedUp.y, direction.y, 0.0f, rightDir.z, fixedUp.z, direction.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

    const glm::vec3 center = rayOrigin + (direction * (rayMaxRange * 0.5f));
    const float radiusScale = rayRadius * 2.0f;

    const glm::mat4 model = Matrix_Translate(center.x, center.y, center.z) * orientation * Matrix_Scale(radiusScale, radiusScale, rayMaxRange);
    RenderHelper::renderModel(modelUniform, colorUniform, model, rayMesh, glm::vec3(1.0f, 0.1f, 0.1f));
}
