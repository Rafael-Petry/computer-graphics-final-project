#ifndef OBJECT_H
#define OBJECT_H

#include <string>

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

class Object
{
public:
    Object(std::string meshPath = "../../src/objects/celestialBody/celestialBody.obj", glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f));
    virtual ~Object() {}

    void update(GLint modelUniform, GLint colorUniform, float currentFrame) const;

protected:
    const std::string meshPath;
    const glm::vec3 color;

    virtual glm::mat4 translate(float currentFrame) const = 0;
    virtual glm::mat4 rotate(float currentFrame) const = 0;
};

#endif
