#ifndef BOUNDING_SPHERE_H
#define BOUNDING_SPHERE_H

#include <glm/vec3.hpp>

#include "collider.h"

class BoundingBox;

class BoundingSphere : public Collider
{
public:
    BoundingSphere();
    BoundingSphere(const glm::vec3 &center, float radius);

    bool isInitialized() const override;
    const glm::vec3 &getCenter() const;
    float getRadius() const;
    bool testCollision(const BoundingSphere &other, const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &otherPosition, const glm::vec3 &otherScale) const;
    bool testCollision(const BoundingBox &other, const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &otherPosition, const glm::vec3 &otherScale) const;

private:
    glm::vec3 center;
    float radius = 0.0f;
    bool initialized = false;
};

#endif
