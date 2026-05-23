#include <algorithm>
#include <cmath>
#include <iostream>

#include <glm/common.hpp>
#include <glm/geometric.hpp>

#include "boundingSphere.h"
#include "boundingBox.h"

namespace {
    float maxAbsComponent(const glm::vec3 &value) { return std::max(std::max(std::fabs(value.x), std::fabs(value.y)), std::fabs(value.z)); }
}

BoundingSphere::BoundingSphere() : center(0.0f), radius(0.0f), initialized(false) {}

BoundingSphere::BoundingSphere(const glm::vec3 &center, float radius) : center(center), radius(radius), initialized(true) {}

bool BoundingSphere::isInitialized() const { return initialized; }

bool BoundingSphere::testCollision(
    const BoundingSphere &other, const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &otherPosition, const glm::vec3 &otherScale) const
{
    if (!initialized || !other.initialized) {
        return false;
    }

    const float radiusA = radius * maxAbsComponent(scale);
    const float radiusB = other.radius * maxAbsComponent(otherScale);

    const glm::vec3 worldCenterA = (center * scale) + position;
    const glm::vec3 worldCenterB = (other.center * otherScale) + otherPosition;

    const glm::vec3 delta = worldCenterA - worldCenterB;
    const float distanceSquared = glm::dot(delta, delta);
    const float combinedRadius = radiusA + radiusB;

    if (distanceSquared <= combinedRadius * combinedRadius) {
        std::cout << "The Objects Collided!" << std::endl;
        return true;
    }

    return false;
}

bool BoundingSphere::testCollision(const BoundingBox &other, const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &otherPosition, const glm::vec3 &otherScale) const
{
    if (!initialized || !other.isInitialized()) {
        return false;
    }

    const float scaledRadius = radius * maxAbsComponent(scale);
    const glm::vec3 worldCenter = (center * scale) + position;

    const glm::vec3 scaledMinB = other.getMin() * otherScale;
    const glm::vec3 scaledMaxB = other.getMax() * otherScale;
    const glm::vec3 minB = glm::min(scaledMinB, scaledMaxB) + otherPosition;
    const glm::vec3 maxB = glm::max(scaledMinB, scaledMaxB) + otherPosition;

    const glm::vec3 closestPoint = glm::clamp(worldCenter, minB, maxB);
    const glm::vec3 delta = worldCenter - closestPoint;
    const float distanceSquared = glm::dot(delta, delta);

    if (distanceSquared <= scaledRadius * scaledRadius) {
        std::cout << "The Objects Collided!" << std::endl;
        return true;
    }

    return false;
}
