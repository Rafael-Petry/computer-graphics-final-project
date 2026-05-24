#ifndef BOUNDING_SPHERE_H
#define BOUNDING_SPHERE_H

#include <glm/vec3.hpp>

#include "collider.h"

class BoundingBox;
class Object;

class BoundingSphere : public Collider
{
public:
    BoundingSphere();
    BoundingSphere(const glm::vec3 &center, float radius);

    bool isInitialized() const override;
    bool testCollisionBoundingSphere(const Object &objectA, const Object &objectB) const;
    bool testCollisionBoundingBox(const Object &sphereObject, const Object &boxObject) const;

private:
    glm::vec3 center;
    float radius = 0.0f;
    bool initialized = false;
};

#endif
