#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <string>

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

class Spaceship
{
public:
    void update(GLint modelUniform, GLint colorUniform, float currentFrame) const;
    static const std::string &getMeshPath();

private:
    glm::mat4 translate(float currentFrame) const;
    glm::mat4 rotate(float currentFrame) const;
    void shoot() const;
    static const std::string meshPath;
};

#endif
