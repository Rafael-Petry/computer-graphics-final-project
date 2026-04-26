#include <iostream>

#include "planet.h"

const std::string Planet::color = "#00FF00";

void Planet::translate()
{
	std::cout << "The planet is orbiting the sun..." << std::endl;
}

void Planet::rotate()
{
	std::cout << "Rotating the planet..." << std::endl;
}

Sun& Planet::getSun()
{
	return sun;
}

const Sun& Planet::getSun() const
{
	return sun;
}
