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

    if (mesh.textureId == 0 && mesh.vao != 0) {
        RenderHelper::generateSunTexture(mesh, 512, 512);
    }

    scaleValue = glm::vec3(0.002f);
    position = glm::vec3(0.0f, 0.0f, 0.0f);
}

Sun &Sun::getInstance()
{
    static Sun instance;
    return instance;
}

void Sun::collide(Window *window)
{
    Spaceship &spaceship = Spaceship::getInstance();

    if (boundingSphere.testCollisionBoundingBox(*this, spaceship)) {
        spaceship.applyDamage(100);
    }
}

glm::mat4 Sun::translate(Window *window) { return Matrix_Translate(position.x, position.y, position.z); }
glm::mat4 Sun::rotate(Window *window) { return Matrix_Rotate_Y(window->getCurrentFrame() * 0.1f); }