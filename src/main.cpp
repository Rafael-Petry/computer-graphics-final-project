#include <iostream>

#include "celestialBody/asteroid/asteroid.h"
#include "celestialBody/planet/planet.h"
#include "celestialBody/sun/sun.h"

#include "spaceship/spaceship.h"

int main()
{
    Spaceship spaceship;

    Asteroid asteroid;
    Sun& sun = Sun::getInstance();
    Planet planet;

    std::cout << "-----Run Spaceship-----" << std::endl;
    spaceship.translate();
    spaceship.rotate();
    spaceship.shoot();
    std::cout << "Model path: " << Spaceship::getModel() << std::endl;
    std::cout << "-----------------------" << std::endl;
    std::cout << std::endl;

    std::cout << "-----Run Celestial Body-----" << std::endl;
    std::cout << "Celestial body model path: " << CelestialBody::getModel() << std::endl;
    std::cout << "----------------------------" << std::endl;
    std::cout << std::endl;

    std::cout << "------Run Asteroid------" << std::endl;
    asteroid.translate();
    asteroid.rotate();
    std::cout << "Asteroid color: " << CelestialBody::getColor() << std::endl;
    std::cout << "------------------------" << std::endl;
    std::cout << std::endl;

    std::cout << "--------Run Sun--------" << std::endl;
    sun.translate();
    sun.rotate();
    std::cout << "Sun color: " << Sun::color << std::endl;
    std::cout << "-----------------------" << std::endl;
    std::cout << std::endl;

    std::cout << "-------Run Planet------" << std::endl;
    planet.translate();
    planet.rotate();
    std::cout << "Planet color: " << Planet::color << std::endl;
    std::cout << "-----------------------" << std::endl;

    return 0;
}