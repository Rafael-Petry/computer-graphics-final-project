#include <iostream>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "sun.h"
#include "../../spaceship/spaceship.h"
#include "../../../helpers/collision/collision.h"
#include "../../../helpers/render/render.h"
#include "../../../window/window.h"
#include "../../vendor/include/matrices.h"

Mesh Sun::mesh;
BoundingSphere Sun::boundingSphere;

Sun::Sun() : CelestialBody(mesh, boundingSphere, glm::vec3(1.0f, 1.0f, 0.0f))
{
    if (mesh.vao == 0) {
        mesh = RenderHelper::loadObjMesh("../../src/objects/celestialBody/sun/sun.obj");
    }

    if (!boundingSphere.isInitialized() && mesh.vao != 0) {
        boundingSphere = CollisionHelper::generateBoundingSphere(mesh);
    }

    scaleValue = glm::vec3(30.0f);
    position = glm::vec3(0.0f, 0.0f, 0.0f);
}

Sun &Sun::getInstance()
{
    static Sun instance;
    return instance;
}

void Sun::collide(Window *window)
{
    const Spaceship &spaceship = Spaceship::getInstance();

    if (boundingSphere.testCollisionBoundingBox(*this, spaceship)) {
        std::cout << "The sun collided with the spaceship!" << std::endl;
    }
}

glm::mat4 Sun::translate(Window *window) { return Matrix_Translate(position.x, position.y, position.z); }
glm::mat4 Sun::rotate(Window *window) { return Matrix_Rotate_Y(window->getCurrentFrame() * 0.1f); }
