#ifndef CELESTIAL_BODY_H
#define CELESTIAL_BODY_H

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "../../helpers/render/mesh.h"
#include "../../helpers/collision/colliders/boundingSphere.h"

#include "../object.h"

class CelestialBody : public Object
{
public:
    CelestialBody(const glm::vec3 &color = glm::vec3(0.5f, 0.5f, 0.5f));
    virtual ~CelestialBody() {}

protected:
    CelestialBody(const Mesh &mesh, const BoundingSphere &boundingSphere, const glm::vec3 &color);

private:
    void shoot() const;

    static Mesh mesh;
    static BoundingSphere boundingSphere;
};

#endif
