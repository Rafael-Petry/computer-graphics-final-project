#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include <glm/vec3.hpp>

#include "collider.h"

class Object;

class BoundingBox : public Collider
{
public:
    BoundingBox();
    BoundingBox(const glm::vec3 &min, const glm::vec3 &max);

    bool isInitialized() const override;
    const glm::vec3 &getMin() const;
    const glm::vec3 &getMax() const;
    bool testCollisionBoundingBox(const Object &objectA, const Object &objectB) const;

private:
    glm::vec3 min;
    glm::vec3 max;
    bool initialized = false;
};

#endif
