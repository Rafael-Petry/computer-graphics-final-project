#ifndef SUN_H
#define SUN_H

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "../../objects/celestialBody/celestialBody.h"
#include "../../../collisions.h"

class Window;

class Sun : public CelestialBody
{
public:
    Sun(const Sun &) = delete;
    Sun &operator=(const Sun &) = delete;

    static Sun &getInstance();

protected:
    void collide(Window *window) override;
    glm::mat4 translate(Window *window) override;
    glm::mat4 rotate(Window *window) override;

private:
    Sun();

    static Mesh mesh;
    static BoundingSphere boundingSphere;
};

#endif
