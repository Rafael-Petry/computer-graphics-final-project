#include <algorithm>
#include <cmath>
#include <random>

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

unsigned int Planet::nextSeed = 0;

Planet::Planet(float orbitRadius) : CelestialBody(instanceMesh, instanceBoundingSphere, color)
{
    // Chaque planète charge son propre mesh et génère une texture unique
    instanceMesh = RenderHelper::loadObjMesh("../../src/objects/celestialBody/planet/planet.obj");

    if (!instanceBoundingSphere.isInitialized() && instanceMesh.vao != 0) {
        instanceBoundingSphere = CollisionHelper::generateBoundingSphere(instanceMesh);
    }

    // Texture procédurale désertique/rocheuse, seed différent par planète
    RenderHelper::generateGrassPlanetTexture(instanceMesh, 512, 512, nextSeed++);

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> phaseDist(0.0f, 2.0f);

    scaleValue = glm::vec3(15.4f);
    position = glm::vec3(0.0f);

    this->orbitRadius = orbitRadius;
    orbitSpeed = 0.01f;

    const std::vector<glm::vec3> colors = {
        glm::vec3(0.2f, 0.6f, 1.0f), glm::vec3(0.9f, 0.4f, 0.2f), glm::vec3(0.4f, 0.9f, 0.5f), glm::vec3(0.8f, 0.8f, 0.3f), glm::vec3(0.7f, 0.5f, 1.0f)};

    orbitPhase = phaseDist(rng);

    std::uniform_real_distribution<float> treeBushDist(0.0f, 1.0f);

    // Spawn trees and bushes on the planet's surface
    const int treesPerPlanet = 30;
    const int bushesPerPlanet = 30;

    for (int t = 0; t < treesPerPlanet; ++t) {
        float z = 2.0f * treeBushDist(rng) - 1.0f; // [-1, 1]
        float a = treeBushDist(rng) * 2.0f * M_PI; // [0, 2π]

        float r = sqrt(1.0f - z * z);

        glm::vec3 offset = glm::vec3(r * cos(a), r * sin(a), z);
        glm::vec3 treeColor = glm::vec3(0.2f, 0.6f, 1.0f) * glm::vec3(0.5f, 1.0f, 0.5f);
        trees.emplace_back(treeColor, this, offset * 15.0f);
    }

    for (int b = 0; b < bushesPerPlanet; ++b) {
        float z = 2.0f * treeBushDist(rng) - 1.0f; // [-1, 1]
        float a = treeBushDist(rng) * 2.0f * M_PI; // [0, 2π]

        float r = sqrt(1.0f - z * z);

        glm::vec3 offset = glm::vec3(r * cos(a), r * sin(a), z);
        glm::vec3 bushColor = glm::vec3(0.2f, 0.6f, 1.0f) * glm::vec3(0.5f, 1.0f, 0.5f);
        bushes.emplace_back(bushColor, this, offset * 15.0f);
    }
}

void Planet::update(GLint modelUniform,
                    GLint colorUniform,
                    Window *window,
                    GLint useTextureUniform,
                    GLint texSamplerUniform,
                    GLint isEmissiveUniform,
                    bool isEmissive,
                    GLint metallicUniform,
                    GLint roughnessUniform,
                    GLint specularUniform)
{
    CelestialBody::update(
        modelUniform, colorUniform, window, useTextureUniform, texSamplerUniform, isEmissiveUniform, isEmissive, metallicUniform, roughnessUniform, specularUniform);

    for (Tree &tree : trees) {
        tree.update(modelUniform, colorUniform, window, useTextureUniform, texSamplerUniform, isEmissiveUniform, false, metallicUniform, roughnessUniform, specularUniform);
    }

    for (Bush &bush : bushes) {
        bush.update(modelUniform, colorUniform, window, useTextureUniform, texSamplerUniform, isEmissiveUniform, false, metallicUniform, roughnessUniform, specularUniform);
    }
}

void Planet::collide(Window *window)
{
    Spaceship &spaceship = Spaceship::getInstance();

    if (instanceBoundingSphere.testCollisionBoundingBox(*this, spaceship)) {
        const glm::vec3 planetScale = getScale();
        const glm::vec3 planetCenter = (instanceBoundingSphere.getCenter() * planetScale) + position;

        const BoundingBox &shipBox = spaceship.getBoundingBox();
        const glm::vec3 shipScale = spaceship.getScale();
        const glm::vec3 shipBoxCenter = (shipBox.getMin() + shipBox.getMax()) * 0.5f;
        const glm::vec3 shipBoxExtents = (shipBox.getMax() - shipBox.getMin()) * 0.5f;
        const glm::vec3 shipCenterOffset = shipBoxCenter * shipScale;

        glm::vec3 shipCenter = spaceship.getPosition() + shipCenterOffset;
        glm::vec3 normal = shipCenter - planetCenter;
        const float normalLength = glm::length(normal);
        if (normalLength < 0.0001f) {
            normal = glm::vec3(0.0f, 1.0f, 0.0f);
        } else {
            normal /= normalLength;
        }

        const float planetRadius = 14.6f;
        const float shipRadius = glm::length(shipBoxExtents * shipScale);
        const float landingPadding = 0.05f;
        const float distanceFromCenter = planetRadius + shipRadius + landingPadding;
        spaceship.landOn(this, normal, distanceFromCenter, shipCenterOffset);
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

    const glm::vec3 curve1[4] = {center + glm::vec3(orbitRadius, 0.0f, 0.0f),
                                 center + glm::vec3(orbitRadius, 0.0f, orbitRadius),
                                 center + glm::vec3(-orbitRadius, 0.0f, orbitRadius),
                                 center + glm::vec3(-orbitRadius, 0.0f, 0.0f)};

    const glm::vec3 curve2[4] = {center + glm::vec3(-orbitRadius, 0.0f, 0.0f),
                                 center + glm::vec3(-orbitRadius, 0.0f, -orbitRadius),
                                 center + glm::vec3(orbitRadius, 0.0f, -orbitRadius),
                                 center + glm::vec3(orbitRadius, 0.0f, 0.0f)};

    if (orbitPhase < 1.0f) {
        position = cubicBezier(curve1[0], curve1[1], curve1[2], curve1[3], orbitPhase);
    } else {
        position = cubicBezier(curve2[0], curve2[1], curve2[2], curve2[3], orbitPhase - 1.0f);
    }

    return Matrix_Translate(position.x, position.y, position.z);
}

glm::mat4 Planet::rotate(Window *window) { return Matrix_Identity(); }