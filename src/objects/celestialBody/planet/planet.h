#ifndef PLANET_H
#define PLANET_H

#include <glm/mat4x4.hpp>

#include "../../objects/celestialBody/celestialBody.h"

class Window;

class Planet : public CelestialBody
{
public:
    Planet(const glm::vec3 &color = glm::vec3(0.0f, 1.0f, 0.0f));

protected:
    glm::mat4 translate(Window *window) override;
    glm::mat4 rotate(Window *window) override;
    glm::mat4 scale(Window *window) override;

private:
    float orbitPhase = 0.0f;
    float orbitSpeed = 0.25f;
    float orbitRadius = 2.0f;

    static Mesh mesh;
};

#endif
