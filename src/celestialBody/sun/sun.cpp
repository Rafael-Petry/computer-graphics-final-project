#include <iostream>

#include "sun.h"

const std::string Sun::color = "#FFFF00";
const std::string Sun::model = "/home/aluno/computer-graphics-final-project/src/celestialBody/sun/sun.obj";

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
