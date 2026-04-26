#include "celestialBody.h"

const std::string CelestialBody::model = "src/celestialBody/celestialBody.obj";
const std::string CelestialBody::color = "#808080";

const std::string& CelestialBody::getModel()
{
	return model;
}

const std::string& CelestialBody::getColor()
{
	return color;
}
