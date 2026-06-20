#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <glm/vec3.hpp>

class Object;

class Collider
{
public:
    virtual ~Collider() = default;
    virtual bool isInitialized() const = 0;
};

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

class BoundingSphere : public Collider
{
public:
    BoundingSphere();
    BoundingSphere(const glm::vec3 &center, float radius);

    bool isInitialized() const override;
    const glm::vec3 &getCenter() const;
    float getRadius() const;
    bool testCollisionBoundingSphere(const Object &objectA, const Object &objectB) const;
    bool testCollisionBoundingBox(const Object &sphereObject, const Object &boxObject) const;
    bool testRay(const Object &sphereObject, const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, float *hitDistance) const;

private:
    glm::vec3 center;
    float radius = 0.0f;
    bool initialized = false;
};

#endif
