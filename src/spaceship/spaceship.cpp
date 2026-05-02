#include "spaceship.h"

#include <iostream>

#include <glm/vec3.hpp>

#include "../helpers/render.h"

const std::string Spaceship::model = "../../src/spaceship/spaceship.obj";

void Spaceship::render(GLint modelUniform, GLint colorUniform, const glm::mat4& modelMatrix) const
{
	RenderHelper::RenderModel(
		modelUniform,
		colorUniform,
		modelMatrix,
		getModel(),
		glm::vec3(0.73f, 0.79f, 0.88f));
}

void Spaceship::translate() const
{
	std::cout << "Moving the spaceship..." << std::endl;
}

void Spaceship::rotate() const
{
	std::cout << "Rotating the spaceship" << std::endl;
}

void Spaceship::shoot() const
{
	std::cout << "The spaceship is shooting..." << std::endl;
}

const std::string& Spaceship::getModel()
{
	return model;
}
