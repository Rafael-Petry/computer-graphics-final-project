#ifndef COLLISION_H
#define COLLISION_H

#include "colliders/boundingBox.h"
#include "../render/mesh.h"

class CollisionHelper
{
public:
    static BoundingBox generateBoundingBox(const Mesh &mesh);
};

#endif
