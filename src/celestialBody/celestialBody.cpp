#include "celestialBody.h"
#include <iostream>

const std::string CelestialBody::model = "src/celestialBody/celestialBody.obj";
const std::string CelestialBody::color = "#808080";

const void CelestialBody::render()
{
	std::cout << "Rendering the celestial body..." << std::endl;
}

const std::string& CelestialBody::getModel()
{
	return model;
}

const std::string& CelestialBody::getColor()
{
	return color;
}
