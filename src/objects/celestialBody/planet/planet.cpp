#include <algorithm>
#include <cmath>

#include <glm/mat4x4.hpp>
#include <glm/geometric.hpp>

#include "planet.h"
#include "../../spaceship/spaceship.h"
#include "../../../helpers/collision/collision.h"
#include "../../../helpers/render/render.h"
#include "../../../window/window.h"
#include "../sun/sun.h"
#include "../../vendor/include/matrices.h"

namespace {
    float maxAbsComponent(const glm::vec3 &value) { return std::max(std::max(std::fabs(value.x), std::fabs(value.y)), std::fabs(value.z)); }

    glm::vec3 cubicBezier(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, float t)
    {
        const float u = 1.0f - t;
        const float tt = t * t;
        const float uu = u * u;
        const float uuu = uu * u;
        const float ttt = tt * t;

        return (p0 * uuu) + (p1 * (3.0f * uu * t)) + (p2 * (3.0f * u * tt)) + (p3 * ttt);
    }
}

Mesh Planet::mesh;
BoundingSphere Planet::boundingSphere;

Planet::Planet(const glm::vec3 &color, float orbitRadius, float orbitSpeed, float orbitPhase) : CelestialBody(mesh, boundingSphere, color)
{
    if (mesh.vao == 0) {
        mesh = RenderHelper::loadObjMesh("../../src/objects/celestialBody/planet/planet.obj");
    }

    if (!boundingSphere.isInitialized() && mesh.vao != 0) {
        boundingSphere = CollisionHelper::generateBoundingSphere(mesh);
    }

    scaleValue = glm::vec3(5.0f);
    position = glm::vec3(0.0f);
    this->orbitRadius = orbitRadius;
    this->orbitSpeed = orbitSpeed;
    this->orbitPhase = orbitPhase;
}

void Planet::collide(Window *window)
{
    const Spaceship &spaceship = Spaceship::getInstance();

    if (boundingSphere.testCollisionBoundingBox(*this, spaceship)) {
        const glm::vec3 planetScale = getScale();
        const glm::vec3 planetCenter = (boundingSphere.getCenter() * planetScale) + position;
        const float planetRadius = boundingSphere.getRadius() * maxAbsComponent(planetScale);

        const BoundingBox &shipBox = spaceship.getBoundingBox();
        const glm::vec3 shipScale = spaceship.getScale();
        const glm::vec3 shipBoxCenter = (shipBox.getMin() + shipBox.getMax()) * 0.5f;
        const glm::vec3 shipBoxExtents = (shipBox.getMax() - shipBox.getMin()) * 0.5f;
        const glm::vec3 shipCenterOffset = shipBoxCenter * shipScale;
        const float shipRadius = glm::length(shipBoxExtents * shipScale);

        glm::vec3 shipCenter = spaceship.getPosition() + shipCenterOffset;
        glm::vec3 normal = shipCenter - planetCenter;
        const float normalLength = glm::length(normal);
        if (normalLength < 0.0001f) {
            normal = glm::vec3(0.0f, 1.0f, 0.0f);
        } else {
            normal /= normalLength;
        }

        const float landingPadding = 0.05f;
        const float distanceFromCenter = planetRadius + shipRadius + landingPadding;
        Spaceship::getInstance().landOn(this, normal, distanceFromCenter, shipCenterOffset);
    }
}

glm::mat4 Planet::translate(Window *window)
{
    orbitPhase += window->getDeltaTime() * orbitSpeed;
    if (orbitPhase >= 2.0f) {
        orbitPhase = fmodf(orbitPhase, 2.0f);
    }

    const Sun &sun = Sun::getInstance();
    const glm::vec3 center = sun.getPosition();
    const float safeOrbitRadius = std::max(orbitRadius, sun.getScale().x + scaleValue.x + 5.0f);

    const glm::vec3 curve1[4] = {center + glm::vec3(safeOrbitRadius, 0.0f, 0.0f),
                                 center + glm::vec3(safeOrbitRadius, 0.0f, safeOrbitRadius),
                                 center + glm::vec3(-safeOrbitRadius, 0.0f, safeOrbitRadius),
                                 center + glm::vec3(-safeOrbitRadius, 0.0f, 0.0f)};

    const glm::vec3 curve2[4] = {center + glm::vec3(-safeOrbitRadius, 0.0f, 0.0f),
                                 center + glm::vec3(-safeOrbitRadius, 0.0f, -safeOrbitRadius),
                                 center + glm::vec3(safeOrbitRadius, 0.0f, -safeOrbitRadius),
                                 center + glm::vec3(safeOrbitRadius, 0.0f, 0.0f)};

    if (orbitPhase < 1.0f) {
        position = cubicBezier(curve1[0], curve1[1], curve1[2], curve1[3], orbitPhase);
    } else {
        position = cubicBezier(curve2[0], curve2[1], curve2[2], curve2[3], orbitPhase - 1.0f);
    }

    return Matrix_Translate(position.x, position.y, position.z);
}

glm::mat4 Planet::rotate(Window *window) { return Matrix_Rotate_Y(window->getCurrentFrame() * 0.01f); }
