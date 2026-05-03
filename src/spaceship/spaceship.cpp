#include <iostream>
#include <string>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "../helpers/render/render.h"
#include "../vendor/include/matrices.h"
#include "spaceship.h"

const std::string Spaceship::meshPath = "../../src/spaceship/spaceship.obj";

void Spaceship::update(GLint modelUniform, GLint colorUniform, float currentFrame) const
{
    const glm::mat4 spaceshipModel = translate(currentFrame) * rotate(currentFrame);
    RenderHelper::renderModel(modelUniform, colorUniform, spaceshipModel, getMeshPath(), glm::vec3(0.73f, 0.79f, 0.88f));
}

glm::mat4 Spaceship::translate(float currentFrame) const { return Matrix_Translate(0.0f, 0.2f, 5.0f); }

glm::mat4 Spaceship::rotate(float currentFrame) const { return Matrix_Rotate_Y(currentFrame); }

void Spaceship::shoot() const { std::cout << "The spaceship is shooting..." << std::endl; }

const std::string &Spaceship::getMeshPath() { return meshPath; }
