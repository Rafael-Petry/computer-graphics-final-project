#ifndef PLANET_H
#define PLANET_H

#include <string>

#include <glm/mat4x4.hpp>

#include "../../objects/celestialBody/celestialBody.h"

class Planet : public CelestialBody
{
public:
    Planet(const std::string &meshPath = "../../src/objects/celestialBody/celestialBody.obj", const glm::vec3 &color = glm::vec3(0.0f, 1.0f, 0.0f));

protected:
    glm::mat4 translate(float currentFrame) const override;
    glm::mat4 rotate(float currentFrame) const override;
    glm::mat4 scale(float currentFrame) const override;
};

#endif
