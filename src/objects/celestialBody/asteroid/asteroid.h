#ifndef ASTEROID_H
#define ASTEROID_H

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include "../../objects/celestialBody/celestialBody.h"

class Window;

class Asteroid : public CelestialBody
{
public:
    Asteroid(const glm::vec3 &color = glm::vec3(0.5f, 0.5f, 0.5f));

protected:
    glm::mat4 translate(Window *window) override;
    glm::mat4 rotate(Window *window) override;
    glm::mat4 scale(Window *window) override;

private:
    glm::vec4 position;
    float chaseSpeed = 1.4f;

    static Mesh mesh;
};

#endif
