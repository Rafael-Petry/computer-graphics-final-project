#include <iostream>

#include "planet.h"

const std::string Planet::color = "#00FF00";
const std::string Planet::model = "src/celestialBody/planet/planet.obj";

void Planet::translate() { std::cout << "The planet is orbiting the sun..." << std::endl; }

void Planet::rotate() { std::cout << "Rotating the planet..." << std::endl; }

const std::string &Planet::getColor() const { return color; }
