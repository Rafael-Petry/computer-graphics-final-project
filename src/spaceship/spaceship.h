#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <iostream>
#include <string>

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "../helpers/render/render.h"

class Spaceship
{
public:
    void update(GLint modelUniform, GLint colorUniform, const glm::mat4 &modelMatrix) const;
    void translate() const;
    void rotate() const;
    void shoot() const;

    static const std::string &getMeshPath();

private:
    static const std::string meshPath;
};

#endif
