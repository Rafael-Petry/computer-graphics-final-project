#include <iostream>

#include "spaceship/spaceship.h"

int main()
{
    Spaceship spaceship;

    spaceship.translate();
    spaceship.rotate();
    spaceship.shoot();

    std::cout << "Model path: " << Spaceship::getModel() << std::endl;

    return 0;
}