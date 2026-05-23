#include <iostream>

#include <glm/common.hpp>

#include "boundingBox.h"

BoundingBox::BoundingBox() : min(0.0f), max(0.0f), initialized(false) {}

BoundingBox::BoundingBox(const glm::vec3 &min, const glm::vec3 &max) : min(min), max(max), initialized(true) {}

bool BoundingBox::isInitialized() const { return initialized; }

const glm::vec3 &BoundingBox::getMin() const { return min; }

const glm::vec3 &BoundingBox::getMax() const { return max; }

bool BoundingBox::testCollision(const BoundingBox &other, const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &otherPosition, const glm::vec3 &otherScale) const
{
    if (!initialized || !other.initialized) {
        return false;
    }

    const glm::vec3 scaledMinA = min * scale;
    const glm::vec3 scaledMaxA = max * scale;
    const glm::vec3 scaledMinB = other.min * otherScale;
    const glm::vec3 scaledMaxB = other.max * otherScale;

    const glm::vec3 minA = glm::min(scaledMinA, scaledMaxA) + position;
    const glm::vec3 maxA = glm::max(scaledMinA, scaledMaxA) + position;
    const glm::vec3 minB = glm::min(scaledMinB, scaledMaxB) + otherPosition;
    const glm::vec3 maxB = glm::max(scaledMinB, scaledMaxB) + otherPosition;

    const bool overlapX = minA.x <= maxB.x && maxA.x >= minB.x;
    const bool overlapY = minA.y <= maxB.y && maxA.y >= minB.y;
    const bool overlapZ = minA.z <= maxB.z && maxA.z >= minB.z;

    if (overlapX && overlapY && overlapZ) {
        std::cout << "The Objects Collided!" << std::endl;
        return true;
    }

    return false;
}
