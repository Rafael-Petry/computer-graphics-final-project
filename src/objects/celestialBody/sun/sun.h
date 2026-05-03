#ifndef SUN_H
#define SUN_H

#include <string>

#include <glm/mat4x4.hpp>

#include "../../objects/celestialBody/celestialBody.h"

class Sun : public CelestialBody
{
public:
    Sun(const Sun &) = delete;
    Sun &operator=(const Sun &) = delete;

    static Sun &getInstance();

protected:
    glm::mat4 translate(float currentFrame) const override;
    glm::mat4 rotate(float currentFrame) const override;
    glm::mat4 scale(float currentFrame) const override;

private:
    Sun();
};

#endif
