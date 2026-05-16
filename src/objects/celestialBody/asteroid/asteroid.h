#ifndef ASTEROID_H
#define ASTEROID_H

#include <string>

#include <glm/mat4x4.hpp>

#include "../../objects/celestialBody/celestialBody.h"

class Window;

class Asteroid : public CelestialBody
{
public:
    Asteroid(const std::string &meshPath = "../../src/objects/celestialBody/asteroid/asteroid.obj", const glm::vec3 &color = glm::vec3(0.5f, 0.5f, 0.5f));

protected:
    glm::mat4 translate(Window *window) const override;
    glm::mat4 rotate(Window *window) const override;
    glm::mat4 scale(Window *window) const override;
};

#endif
