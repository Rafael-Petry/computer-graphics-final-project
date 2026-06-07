#include <algorithm>
#include <cmath>

#include <glm/mat4x4.hpp>
#include <glm/geometric.hpp>

#include "tree.h"
#include "../../helpers/collision/collision.h"
#include "../../helpers/render/render.h"
#include "../../window/window.h"
#include "../../objects/spaceship/spaceship.h"
#include "../../objects/celestialBody/planet/planet.h"
#include "../../vendor/include/matrices.h"

namespace {
    float maxAbsComponent(const glm::vec3 &value) { return std::max(std::max(std::fabs(value.x), std::fabs(value.y)), std::fabs(value.z)); }
}

Mesh Tree::mesh;
BoundingBox Tree::boundingBox;

Tree::Tree(const glm::vec3 &color, Planet *planet, const glm::vec3 &offset, float scale) : Object(mesh, boundingBox, color), planet(planet), offset(offset)
{
    if (mesh.vao == 0) {
        mesh = RenderHelper::loadObjMesh("../../src/objects/tree/tree.obj");
    }

    if (!boundingBox.isInitialized() && mesh.vao != 0) {
        boundingBox = CollisionHelper::generateBoundingBox(mesh);
    }

    scaleValue = glm::vec3(scale);
    this->position = glm::vec3(0.0f);
}

void Tree::collide(Window *window)
{
    const Spaceship &spaceship = Spaceship::getInstance();

    if (boundingBox.testCollisionBoundingBox(*this, spaceship)) {
        glm::vec3 currentPosition = (planet != nullptr) ? planet->getPosition() + offset : position;
        const glm::vec3 treeScale = getScale();
        const glm::vec3 treeCenter = (boundingBox.getMin() + boundingBox.getMax()) * 0.5f * treeScale + currentPosition;
        const glm::vec3 treeExtents = (boundingBox.getMax() - boundingBox.getMin()) * 0.5f * treeScale;
        const float treeRadius = glm::length(treeExtents);

        const BoundingBox &shipBox = spaceship.getBoundingBox();
        const glm::vec3 shipScale = spaceship.getScale();
        const glm::vec3 shipBoxCenter = (shipBox.getMin() + shipBox.getMax()) * 0.5f;
        const glm::vec3 shipBoxExtents = (shipBox.getMax() - shipBox.getMin()) * 0.5f;
        const glm::vec3 shipCenterOffset = shipBoxCenter * shipScale;
        const float shipRadius = glm::length(shipBoxExtents * shipScale);

        glm::vec3 shipCenter = spaceship.getPosition() + shipCenterOffset;
        glm::vec3 normal = shipCenter - treeCenter;
        const float normalLength = glm::length(normal);
        if (normalLength < 0.0001f) {
            normal = glm::vec3(0.0f, 1.0f, 0.0f);
        } else {
            normal /= normalLength;
        }

        const float bumpDistance = treeRadius + shipRadius + 0.1f;
        const glm::vec3 bumpPosition = treeCenter + (normal * bumpDistance);
        Spaceship::getInstance().setPosition(bumpPosition);
    }
}

glm::mat4 Tree::translate(Window *window)
{
    glm::vec3 currentPosition = (planet != nullptr) ? planet->getPosition() + offset : position;
    return Matrix_Translate(currentPosition.x, currentPosition.y, currentPosition.z);
}

glm::mat4 Tree::rotate(Window *window) { return Matrix_Rotate_Y(0.0f); }
