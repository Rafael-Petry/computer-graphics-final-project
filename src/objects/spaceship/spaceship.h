#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <string>

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include "../object.h"

class Spaceship : public Object
{
public:
    Spaceship(const std::string &meshPath = "../../src/objects/spaceship/spaceship.obj", const glm::vec3 &color = glm::vec3(0.73f, 0.79f, 0.88f));

protected:
    glm::mat4 translate(float currentFrame) const override;
    glm::mat4 rotate(float currentFrame) const override;
    glm::mat4 scale(float currentFrame) const override;

private:
    void shoot() const;
};

#endif
