#include <iostream>
#include <glm/geometric.hpp>
#include "asteroid.h"
#include "../../../helpers/collision/collision.h"
#include "../../../helpers/render/render.h"
#include "../../spaceship/spaceship.h"
#include "../../../window/window.h"
#include "../../vendor/include/matrices.h"

Mesh Asteroid::mesh;
BoundingSphere Asteroid::boundingSphere;

Asteroid::Asteroid(const glm::vec3 &color) : CelestialBody(mesh, boundingSphere, color)
{
    if (mesh.vao == 0) {
        mesh = RenderHelper::loadObjMesh("../../src/objects/celestialBody/asteroid/asteroid.obj");
    }

    if (!boundingSphere.isInitialized() && mesh.vao != 0) {
        boundingSphere = CollisionHelper::generateBoundingSphere(mesh);
    }

    scaleValue = glm::vec3(0.1f);
    position = glm::vec3(2.7f, 0.4f, 0.0f);
}

glm::mat4 Asteroid::translate(Window *window)
{
    const glm::vec3 target = Spaceship::getInstance().getPosition();
    glm::vec3 direction = target - position;

    const float distance = glm::length(direction);
    if (distance > 0.001f) {
        direction /= distance;
        position += direction * (chaseSpeed * window->getDeltaTime());
    }

    return Matrix_Translate(position.x, position.y, position.z);
}
glm::mat4 Asteroid::rotate(Window *window) { return Matrix_Rotate_Y(window->getCurrentFrame() * 1.7f); }

void Asteroid::collide(Window *window)
{
    const Spaceship &spaceship = Spaceship::getInstance();

    if (boundingSphere.testCollisionBoundingBox(*this, spaceship)) {
        std::cout << "An asteroid collided with the spaceship!" << std::endl;
    }
}

void Asteroid::onShotHit()
{
    std::cout << "Asteroid hit by a shot." << std::endl;
    position = glm::vec3(-2.8f, 0.6f, -1.5f);
}
