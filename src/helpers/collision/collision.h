#ifndef COLLISION_H
#define COLLISION_H

#include "../../collisions.h"
#include "../render/mesh.h"

class CollisionHelper
{
public:
    static BoundingBox generateBoundingBox(const Mesh &mesh);
    static BoundingSphere generateBoundingSphere(const Mesh &mesh);
};

#endif
