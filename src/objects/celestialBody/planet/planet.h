#ifndef PLANET_H
#define PLANET_H

#include <string>

#include <glm/mat4x4.hpp>

#include "../../objects/celestialBody/celestialBody.h"

class Window;

class Planet : public CelestialBody
{
public:
    Planet(const std::string &meshPath = "../../src/objects/celestialBody/planet/planet.obj", const glm::vec3 &color = glm::vec3(0.0f, 1.0f, 0.0f));

protected:
    glm::mat4 translate(Window *window) const override;
    glm::mat4 rotate(Window *window) const override;
    glm::mat4 scale(Window *window) const override;
};

#endif
