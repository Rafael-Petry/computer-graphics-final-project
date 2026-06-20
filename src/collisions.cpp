#include <algorithm>
#include <cmath>
#include <list>

#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include "collisions.h"
#include "objects/object.h"
#include "objects/celestialBody/asteroid/asteroid.h"
#include "objects/spaceship/spaceship.h"

////// Bounding Box definition //////
BoundingBox::BoundingBox() : min(0.0f), max(0.0f), initialized(false) {}
BoundingBox::BoundingBox(const glm::vec3 &min, const glm::vec3 &max) : min(min), max(max), initialized(true) {}
bool BoundingBox::isInitialized() const { return initialized; }
const glm::vec3 &BoundingBox::getMin() const { return min; }
const glm::vec3 &BoundingBox::getMax() const { return max; }

bool BoundingBox::testCollisionBoundingBox(const Object &objectA, const Object &objectB) const
{
    const auto *otherBox = dynamic_cast<const BoundingBox *>(&objectB.getCollider());
    if (!initialized || otherBox == nullptr || !otherBox->isInitialized()) {
        return false;
    }

    const glm::vec3 scaleA = objectA.getScale();
    const glm::vec3 scaleB = objectB.getScale();
    const glm::vec3 positionA = objectA.getPosition();
    const glm::vec3 positionB = objectB.getPosition();

    const glm::vec3 scaledMinA = min * scaleA;
    const glm::vec3 scaledMaxA = max * scaleA;
    const glm::vec3 scaledMinB = otherBox->min * scaleB;
    const glm::vec3 scaledMaxB = otherBox->max * scaleB;

    const glm::vec3 minA = glm::min(scaledMinA, scaledMaxA) + positionA;
    const glm::vec3 maxA = glm::max(scaledMinA, scaledMaxA) + positionA;
    const glm::vec3 minB = glm::min(scaledMinB, scaledMaxB) + positionB;
    const glm::vec3 maxB = glm::max(scaledMinB, scaledMaxB) + positionB;

    const bool overlapX = minA.x <= maxB.x && maxA.x >= minB.x;
    const bool overlapY = minA.y <= maxB.y && maxA.y >= minB.y;
    const bool overlapZ = minA.z <= maxB.z && maxA.z >= minB.z;

    if (overlapX && overlapY && overlapZ) {
        return true;
    }

    return false;
}

////// Bounding Sphere definition //////
namespace {
    float maxAbsComponent(const glm::vec3 &value) { return std::max(std::max(std::fabs(value.x), std::fabs(value.y)), std::fabs(value.z)); }
}

BoundingSphere::BoundingSphere() : center(0.0f), radius(0.0f), initialized(false) {}
BoundingSphere::BoundingSphere(const glm::vec3 &center, float radius) : center(center), radius(radius), initialized(true) {}
bool BoundingSphere::isInitialized() const { return initialized; }
const glm::vec3 &BoundingSphere::getCenter() const { return center; }
float BoundingSphere::getRadius() const { return radius; }

bool BoundingSphere::testCollisionBoundingSphere(const Object &objectA, const Object &objectB) const
{
    const auto *otherSphere = dynamic_cast<const BoundingSphere *>(&objectB.getCollider());
    if (!initialized || otherSphere == nullptr || !otherSphere->isInitialized()) {
        return false;
    }

    const glm::vec3 scaleA = objectA.getScale();
    const glm::vec3 scaleB = objectB.getScale();
    const glm::vec3 positionA = objectA.getPosition();
    const glm::vec3 positionB = objectB.getPosition();

    const float radiusA = radius * maxAbsComponent(scaleA);
    const float radiusB = otherSphere->radius * maxAbsComponent(scaleB);

    const glm::vec3 worldCenterA = (center * scaleA) + positionA;
    const glm::vec3 worldCenterB = (otherSphere->center * scaleB) + positionB;

    const glm::vec3 delta = worldCenterA - worldCenterB;
    const float distanceSquared = glm::dot(delta, delta);
    const float combinedRadius = radiusA + radiusB;

    if (distanceSquared <= combinedRadius * combinedRadius) {
        return true;
    }

    return false;
}

bool BoundingSphere::testCollisionBoundingBox(const Object &sphereObject, const Object &boxObject) const
{
    const auto *otherBox = dynamic_cast<const BoundingBox *>(&boxObject.getCollider());
    if (!initialized || otherBox == nullptr || !otherBox->isInitialized()) {
        return false;
    }

    const glm::vec3 sphereScale = sphereObject.getScale();
    const glm::vec3 spherePosition = sphereObject.getPosition();
    const glm::vec3 boxScale = boxObject.getScale();
    const glm::vec3 boxPosition = boxObject.getPosition();

    const float scaledRadius = radius * maxAbsComponent(sphereScale);
    const glm::vec3 worldCenter = (center * sphereScale) + spherePosition;

    const glm::vec3 scaledMinB = otherBox->getMin() * boxScale;
    const glm::vec3 scaledMaxB = otherBox->getMax() * boxScale;
    const glm::vec3 minB = glm::min(scaledMinB, scaledMaxB) + boxPosition;
    const glm::vec3 maxB = glm::max(scaledMinB, scaledMaxB) + boxPosition;

    const glm::vec3 closestPoint = glm::clamp(worldCenter, minB, maxB);
    const glm::vec3 delta = worldCenter - closestPoint;
    const float distanceSquared = glm::dot(delta, delta);

    if (distanceSquared <= scaledRadius * scaledRadius) {
        return true;
    }

    return false;
}

bool BoundingSphere::testRay(const Object &sphereObject, const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, float *hitDistance) const
{
    if (!initialized) {
        return false;
    }

    const glm::vec3 sphereScale = sphereObject.getScale();
    const glm::vec3 spherePosition = sphereObject.getPosition();

    const float scaledRadius = radius * maxAbsComponent(sphereScale);
    const glm::vec3 worldCenter = (center * sphereScale) + spherePosition;

    const glm::vec3 originToCenter = rayOrigin - worldCenter;
    const float b = glm::dot(originToCenter, rayDirection);
    const float c = glm::dot(originToCenter, originToCenter) - (scaledRadius * scaledRadius);

    const float discriminant = (b * b) - c;
    if (discriminant < 0.0f) {
        return false;
    }

    const float sqrtDisc = std::sqrt(discriminant);
    float t = -b - sqrtDisc;
    if (t < 0.0f) {
        t = -b + sqrtDisc;
    }

    if (t < 0.0f) {
        return false;
    }

    if (hitDistance != nullptr) {
        *hitDistance = t;
    }

    return true;
}

////// Ray collision //////
void collideAsteroidRay(Spaceship &spaceship, std::list<Asteroid> &asteroids, const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection)
{
    for (Asteroid &asteroid : asteroids) {
        const BoundingSphere *sphere = dynamic_cast<const BoundingSphere *>(&asteroid.getCollider());
        if (sphere == nullptr) {
            continue;
        }

        float hitDistance = 0.0f;
        if (sphere->testRay(asteroid, rayOrigin, rayDirection, &hitDistance)) {
            asteroid.destroy();
            spaceship.addScore(100);
        }
    }
}
