#include <iostream>
#include <string>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "../helpers/render/render.h"
#include "spaceship.h"

const std::string Spaceship::meshPath = "../../src/spaceship/spaceship.obj";

void Spaceship::update(GLint modelUniform, GLint colorUniform, const glm::mat4 &modelMatrix) const
{
    RenderHelper::renderModel(modelUniform, colorUniform, modelMatrix, getMeshPath(), glm::vec3(0.73f, 0.79f, 0.88f));
}

void Spaceship::translate() const { std::cout << "Moving the spaceship..." << std::endl; }

void Spaceship::rotate() const { std::cout << "Rotating the spaceship" << std::endl; }

void Spaceship::shoot() const { std::cout << "The spaceship is shooting..." << std::endl; }

const std::string &Spaceship::getMeshPath() { return meshPath; }
