#ifndef CELESTIAL_BODY_H
#define CELESTIAL_BODY_H

#include <string>

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include "../../helpers/render/render.h"

#include "../object.h"

class CelestialBody : public Object
{
public:
    CelestialBody(const std::string &meshPath = "../../src/objects/celestialBody/celestialBody.obj", const glm::vec3 &color = glm::vec3(0.5f, 0.5f, 0.5f));
    virtual ~CelestialBody() {}

private:
    void shoot() const;
};

#endif
