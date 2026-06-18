#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <glad/glad.h>
#include <list>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "../../helpers/collision/colliders/boundingBox.h"
#include "../celestialBody/asteroid/asteroid.h"
#include "../object.h"

class Window;
class Planet;

class Spaceship : public Object
{
public:
    Spaceship(const Spaceship &) = delete;
    Spaceship &operator=(const Spaceship &) = delete;

    static Spaceship &getInstance();

    void update(GLint modelUniform,
                GLint colorUniform,
                Window *window,
                GLint useTextureUniform = -1,
                GLint texSamplerUniform = -1,
                GLint isEmissiveUniform = -1,
                bool isEmissive = false,
                GLint metallicUniform = -1,
                GLint roughnessUniform = -1,
                GLint specularUniform = -1);
    void updateRotation(Window *window);
    static void updateView(GLFWwindow *window, double xpos, double ypos);

    void addScore(int amount);
    void applyDamage(int amount);
    void landOn(const Planet *planet, const glm::vec3 &surfaceNormal, float distanceFromCenter, const glm::vec3 &shipCenterOffset);

    int getScore() const;
    int getHealth() const;
    glm::mat4 getViewMatrix() const;
    glm::vec3 getCameraPosition() const;
    glm::vec3 getFrontVector() const;
    const BoundingBox &getBoundingBox() const;
    const bool getIsLanded() const { return isLanded; }
    float getInvencibilityTimer() const { return invencibilityTimer; }

    void setVelocity(const glm::vec4 &newVelocity) { velocity = newVelocity; }
    void setInvencibilityTimer(float timer) { invencibilityTimer = timer; }

    void toggleCameraMode() { cameraIsFirstPerson = !cameraIsFirstPerson; }

protected:
    glm::mat4 translate(Window *window) override;
    glm::mat4 rotate(Window *window) override;

private:
    Spaceship(const glm::vec3 &color = glm::vec3(0.73f, 0.79f, 0.88f));

    static constexpr int maxHealth = 5;

    void shoot(Window *window, std::list<Asteroid> &asteroids);
    void renderCrosshair(GLint modelUniform, GLint colorUniform, GLint isEmissiveUniform) const;
    void renderRay(GLint modelUniform, GLint colorUniform, GLint isEmissiveUniform) const;
    glm::mat4 getOrientationMatrix() const;
    void updateOrientation(float deltaYaw, float deltaPitch, float deltaRoll);
    void updateShooting(GLint modelUniform, GLint colorUniform, Window *window, GLint isEmissiveUniform = -1);

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
    float maxMovementSpeed = 20.0f;
    float movementDrag = 0.2f;
    float movementVelocityEpsilon = 0.005f;

    float mouseSensitivity = 0.12f;
    bool firstMouseUpdate = true;
    float lastMouseX = 0.0f;
    float lastMouseY = 0.0f;

    bool isRolling = false;

    float lastShotTime = -1000.0f;
    float shotCooldown = 1.0f;
    float rayVisibleUntil = -1000.0f;
    float rayVisibleDuration = 0.12f;
    float rayMaxRange = 50.0f;
    float rayRadius = 0.02f;
    float crosshairDistance = 5.0f;
    float crosshairLength = 0.08f;
    float crosshairThickness = 0.008f;

    glm::vec3 rayOrigin = glm::vec3(0.0f);
    glm::vec3 rayDirection = glm::vec3(0.0f, 0.0f, -1.0f);

    int score = 0;
    int health = maxHealth;

    bool isLanded = false;
    const Planet *landedPlanet = nullptr;
    glm::vec3 landedNormal = glm::vec3(0.0f, 1.0f, 0.0f);
    float landedDistance = 0.0f;
    glm::vec3 landedShipCenterOffset = glm::vec3(0.0f);
    glm::vec4 landedFront = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    glm::vec4 landedUp = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    glm::vec4 landedRight = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);

    bool cameraIsFirstPerson = false;

    float invencibilityTimer = 0.0f;
    float takeOffSpeed = 0.0f;

    static Mesh mesh;
    static BoundingBox boundingBox;
    static Mesh crosshairMesh;
    static Mesh rayMesh;
};

#endif