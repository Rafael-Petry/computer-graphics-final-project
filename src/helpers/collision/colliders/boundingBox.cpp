#include <iostream>

#include <glm/common.hpp>

#include "boundingBox.h"
#include "../../../objects/object.h"

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
        std::cout << "The Objects Collided!" << std::endl;
        return true;
    }

    return false;
}
