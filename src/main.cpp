#include <iostream>

#include "celestialBody/asteroid/asteroid.h"
#include "celestialBody/planet/planet.h"
#include "celestialBody/sun/sun.h"

int main()
{
    Asteroid asteroid;
    Sun sun;
    Planet planet;

    asteroid.translate();
    asteroid.rotate();

    sun.translate();
    sun.rotate();

    planet.translate();
    planet.rotate();
    planet.getSun().rotate();

    std::cout << "Celestial body model path: " << CelestialBody::getModel() << std::endl;

    return 0;
}