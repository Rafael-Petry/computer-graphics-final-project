#ifndef SUN_H
#define SUN_H

#include <string>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "../../objects/celestialBody/celestialBody.h"

class Window;

class Sun : public CelestialBody
{
public:
    Sun(const Sun &) = delete;
    Sun &operator=(const Sun &) = delete;

    static Sun &getInstance();

    glm::vec3 getPosition() const;

protected:
    glm::mat4 translate(Window *window) override;
    glm::mat4 rotate(Window *window) override;
    glm::mat4 scale(Window *window) override;

private:
    Sun();

    glm::vec3 position;
};

#endif
