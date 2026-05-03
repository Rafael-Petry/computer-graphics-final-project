#ifndef ASTEROID_H
#define ASTEROID_H

#include <string>

#include "../celestialBody.h"

class Asteroid : public CelestialBody
{
public:
    void translate() override;
    void rotate() override;

    static const std::string meshPath;
    static const std::string color;
};

#endif
