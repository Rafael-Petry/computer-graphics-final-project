#include <glad/glad.h>
#include <glm/vec3.hpp>

#include "../../helpers/render/render.h"
#include "celestialBody.h"
#include "../../helpers/colliderGenerator/colliderGenerator.h"
#include "../../vendor/include/matrices.h"

Mesh CelestialBody::mesh;
BoundingSphere CelestialBody::boundingSphere;

CelestialBody::CelestialBody(const glm::vec3 &color) : Object(mesh, boundingSphere, color)
{
    if (mesh.vao == 0) {
        mesh = RenderHelper::loadObjMesh("../../src/objects/celestialBody/celestialBody.obj");
    }

    if (!boundingSphere.isInitialized() && mesh.vao != 0) {
        boundingSphere = ColliderGenerator::generateBoundingSphere(mesh);
    }
}

CelestialBody::CelestialBody(const Mesh &mesh, const BoundingSphere &boundingSphere, const glm::vec3 &color) : Object(mesh, boundingSphere, color) {}
