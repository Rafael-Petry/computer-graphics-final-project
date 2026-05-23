#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include <glm/vec3.hpp>

class BoundingBox
{
public:
    BoundingBox();
    BoundingBox(const glm::vec3 &min, const glm::vec3 &max);

    bool isInitialized() const;
    bool testCollision(const BoundingBox &other, const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &otherPosition, const glm::vec3 &otherScale) const;

private:
    glm::vec3 min;
    glm::vec3 max;
    bool initialized = false;
};

#endif
