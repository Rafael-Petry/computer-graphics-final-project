#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <string>

#include <glad/glad.h>

#include <glm/mat4x4.hpp>

class Spaceship
{
public:
    void render(GLint modelUniform, GLint colorUniform, const glm::mat4 &modelMatrix) const;
    void translate() const;
    void rotate() const;
    void shoot() const;

    static const std::string &getModel();

private:
    static const std::string model;
};

#endif
