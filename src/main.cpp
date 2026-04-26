#include <iostream>

#include "celestialBody/asteroid/asteroid.h"
#include "celestialBody/planet/planet.h"
#include "celestialBody/sun/sun.h"

int main()
{
    Asteroid asteroid;
    Sun& sun = Sun::getInstance();
    Planet planet;

    asteroid.translate();
    asteroid.rotate();

    sun.translate();
    sun.rotate();

    planet.translate();
    planet.rotate();

    std::cout << "Celestial body model path: " << CelestialBody::getModel() << std::endl;
    std::cout << "Celestial body default color: " << CelestialBody::getColor() << std::endl;

    return 0;
}