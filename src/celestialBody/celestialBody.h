#ifndef CELESTIAL_BODY_H
#define CELESTIAL_BODY_H

#include <string>

#include <glad/glad.h>

#include <glm/mat4x4.hpp>

class CelestialBody
{
public:
    virtual ~CelestialBody() {}

    virtual void translate() = 0;
    virtual void rotate() = 0;
    virtual const std::string &getColor() const;

    void update(GLint modelUniform, GLint colorUniform, const glm::mat4 &modelMatrix) const;

    static const std::string &getMeshPath();

protected:
    static const std::string meshPath;
    static const std::string color;
};

#endif
