#include <glad/glad.h>
#include <glm/vec3.hpp>

#include "../../helpers/render/render.h"
#include "celestialBody.h"
#include "../../vendor/include/matrices.h"

Mesh CelestialBody::mesh;

CelestialBody::CelestialBody(const glm::vec3 &color) : Object(mesh, color)
{
    if (mesh.vao == 0) {
        mesh = RenderHelper::loadObjMesh("../../src/objects/celestialBody/celestialBody.obj");
    }
}

CelestialBody::CelestialBody(const Mesh &mesh, const glm::vec3 &color) : Object(mesh, color) {}
