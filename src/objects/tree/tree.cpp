#include <algorithm>
#include <cmath>

#include <glm/mat4x4.hpp>
#include <glm/geometric.hpp>

#include "tree.h"
#include "../../helpers/colliderGenerator/colliderGenerator.h"
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

Tree::Tree(Planet *planet, const glm::vec3 &offset) : Object(mesh, boundingBox, color), planet(planet), offset(offset)
{
    if (mesh.vao == 0 && !mesh.hasSubMeshes()) {
        mesh = RenderHelper::loadObjMesh("../../src/objects/tree/mesh/tree.obj");
        boundingBox = BoundingBox(glm::vec3(-1.0f), glm::vec3(2.0f));
    }

    scaleValue = glm::vec3(2.0f);
    position = (planet != nullptr) ? planet->getPosition() + offset : glm::vec3(0.0f);
}

void Tree::collide(Window *window) { collideTreeWithSpaceship(*this, Spaceship::getInstance()); }

glm::mat4 Tree::translate(Window *window)
{
    position = (planet != nullptr) ? planet->getPosition() + offset : position;
    return Matrix_Translate(position.x, position.y, position.z);
}

glm::mat4 Tree::rotate(Window *window)
{
    if (planet == nullptr) {
        return Matrix_Identity();
    }

    glm::vec3 up = glm::normalize(position - planet->getPosition());

    glm::vec3 worldRight = glm::vec3(1, 0, 0);
    glm::vec3 worldFront = glm::vec3(0, 0, -1);

    glm::vec3 right = glm::normalize(glm::cross(worldFront, up));

    if (glm::length(right) < 0.0001f)
        right = glm::normalize(glm::cross(worldRight, up));

    glm::vec3 front = glm::normalize(glm::cross(up, right));

    return Matrix(right.x, up.x, front.x, 0.0f, right.y, up.y, front.y, 0.0f, right.z, up.z, front.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}
