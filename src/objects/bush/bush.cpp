#include <glm/mat4x4.hpp>

#include "bush.h"
#include "../../helpers/collision/collision.h"
#include "../../helpers/collision/colliders/boundingBox.h"
#include "../../helpers/render/render.h"
#include "../../window/window.h"
#include "../../objects/celestialBody/planet/planet.h"
#include "../../vendor/include/matrices.h"

Mesh Bush::mesh;
BoundingBox Bush::boundingBox;

Bush::Bush(const glm::vec3 &color, Planet *planet, const glm::vec3 &offset, float scale) : Object(mesh, boundingBox, color), planet(planet), offset(offset)
{
    if (mesh.vao == 0) {
        mesh = RenderHelper::loadObjMesh("../../src/objects/bush/bush.obj");
    }

    if (!boundingBox.isInitialized() && mesh.vao != 0) {
        boundingBox = CollisionHelper::generateBoundingBox(mesh);
    }

    scaleValue = glm::vec3(scale);
    this->position = glm::vec3(0.0f);
}

void Bush::collide(Window *window)
{
    // Bushes have no collision detection
}

glm::mat4 Bush::translate(Window *window)
{
    glm::vec3 currentPosition = (planet != nullptr) ? planet->getPosition() + offset : position;
    return Matrix_Translate(currentPosition.x, currentPosition.y, currentPosition.z);
}

glm::mat4 Bush::rotate(Window *window) { return Matrix_Rotate_Y(0.0f); }
