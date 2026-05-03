#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <string>

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include "../object.h"

class Spaceship : public Object
{
public:
    Spaceship();

protected:
    glm::mat4 translate(float currentFrame) const override;
    glm::mat4 rotate(float currentFrame) const override;

private:
    void shoot() const;
};

#endif
