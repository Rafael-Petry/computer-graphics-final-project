#include <glad/glad.h>
#include <glm/vec3.hpp>

#include "../../helpers/render/render.h"
#include "celestialBody.h"
#include "../../helpers/collision/collision.h"
#include "../../vendor/include/matrices.h"

Mesh CelestialBody::mesh;
BoundingBox CelestialBody::boundingBox;

CelestialBody::CelestialBody(const glm::vec3 &color) : Object(mesh, boundingBox, color)
{
    if (mesh.vao == 0) {
        mesh = RenderHelper::loadObjMesh("../../src/objects/celestialBody/celestialBody.obj");
    }

    if (!boundingBox.isInitialized() && mesh.vao != 0) {
        boundingBox = CollisionHelper::generateBoundingBox(mesh);
    }
}

CelestialBody::CelestialBody(const Mesh &mesh, const BoundingBox &boundingBox, const glm::vec3 &color) : Object(mesh, boundingBox, color) {}
