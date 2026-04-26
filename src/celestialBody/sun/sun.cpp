#include <iostream>

#include "sun.h"

const std::string Sun::color = "#FFFF00";

Sun& Sun::getInstance()
{
	static Sun instance;
	return instance;
}

void Sun::translate()
{
	std::cout << "The sun stands still..." << std::endl;
}

void Sun::rotate()
{
	std::cout << "Rotating the sun..." << std::endl;
}

const std::string& Sun::getColor() const
{
	return color;
}
