#include "spaceship.h"

#include <iostream>

const std::string Spaceship::model = "src/spaceship/spaceship.obj";

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
