#include <algorithm>
#include <cmath>

#include <glm/common.hpp>
#include <glm/geometric.hpp>

#include "boundingSphere.h"
#include "boundingBox.h"
#include "../../../objects/object.h"

namespace {
    float maxAbsComponent(const glm::vec3 &value) { return std::max(std::max(std::fabs(value.x), std::fabs(value.y)), std::fabs(value.z)); }
}

BoundingSphere::BoundingSphere() : center(0.0f), radius(0.0f), initialized(false) {}

BoundingSphere::BoundingSphere(const glm::vec3 &center, float radius) : center(center), radius(radius), initialized(true) {}

bool BoundingSphere::isInitialized() const { return initialized; }

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
