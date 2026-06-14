#ifndef PLANET_H
#define PLANET_H

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "../../objects/celestialBody/celestialBody.h"
#include "../../../helpers/collision/colliders/boundingSphere.h"
#include "../../objects/tree/tree.h"
#include "../../objects/bush/bush.h"
class Window;

class Planet : public CelestialBody
{
public:
    Planet(float orbitRadius = 2.0f);

    void update(GLint modelUniform,
                GLint colorUniform,
                Window *window,
                GLint useTextureUniform = -1,
                GLint texSamplerUniform = -1,
                GLint isEmissiveUniform = -1,
                bool isEmissive = false,
                GLint metallicUniform = -1,
                GLint roughnessUniform = -1,
                GLint specularUniform = -1);

protected:
    void collide(Window *window) override;
    glm::mat4 translate(Window *window) override;
    glm::mat4 rotate(Window *window) override;

private:
    float orbitPhase = 0.0f;
    float orbitSpeed = 0.25f;
    float orbitRadius = 2.0f;

    // Mesh et collider propres à chaque instance (texture unique par planète)
    Mesh instanceMesh;
    BoundingSphere instanceBoundingSphere;

    std::vector<Tree> trees;
    std::vector<Bush> bushes;

    static unsigned int nextSeed;
};

#endif