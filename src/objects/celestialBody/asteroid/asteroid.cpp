#include <iostream>
#include "asteroid.h"
#include "../../../helpers/collision/collision.h"
#include "../../../helpers/render/render.h"
#include "../../spaceship/spaceship.h"
#include "../../../window/window.h"
#include "../../vendor/include/matrices.h"

Mesh Asteroid::mesh;
BoundingSphere Asteroid::boundingSphere;

Asteroid::Asteroid(const glm::vec3 &color) : CelestialBody(mesh, boundingSphere, color), position(2.7f, 0.4f, 0.0f, 1.0f)
{
    if (mesh.vao == 0) {
        mesh = RenderHelper::loadObjMesh("../../src/objects/celestialBody/asteroid/asteroid.obj");
    }

    if (!boundingSphere.isInitialized() && mesh.vao != 0) {
        boundingSphere = CollisionHelper::generateBoundingSphere(mesh);
    }
}

glm::mat4 Asteroid::translate(Window *window)
{
    const glm::vec4 target = Spaceship::getInstance().getPosition();
    glm::vec4 direction = target - position;
    direction.w = 0.0f;

    const float distance = norm(direction);
    if (distance > 0.001f) {
        direction /= distance;
        position += direction * (chaseSpeed * window->getDeltaTime());
    }

    const Spaceship &spaceship = Spaceship::getInstance();
    boundingSphere.testCollision(spaceship.getBoundingBox(), glm::vec3(position), glm::vec3(0.1f), glm::vec3(spaceship.getPosition()), glm::vec3(0.3f));

    return Matrix_Translate(position.x, position.y, position.z);
}
glm::mat4 Asteroid::rotate(Window *window) { return Matrix_Rotate_Y(window->getCurrentFrame() * 1.7f); }
glm::mat4 Asteroid::scale(Window *window) { return Matrix_Scale(0.1f, 0.1f, 0.1f); }
