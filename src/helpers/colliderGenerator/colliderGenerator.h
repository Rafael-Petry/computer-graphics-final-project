#ifndef COLLISION_H
#define COLLISION_H

#include "../../collisions.h"
#include "../render/mesh.h"

class ColliderGenerator
{
public:
    static BoundingBox generateBoundingBox(const Mesh &mesh);
    static BoundingSphere generateBoundingSphere(const Mesh &mesh);
};

#endif
