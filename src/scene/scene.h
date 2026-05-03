#ifndef SCENE_H
#define SCENE_H

#include "../celestialBody/asteroid/asteroid.h"
#include "../celestialBody/planet/planet.h"
#include "../celestialBody/sun/sun.h"
#include "../spaceship/spaceship.h"

class Scene
{
public:
    Scene();
    void render(GLint modelUniform, GLint colorUniform, float currentFrame) const;

private:
    Spaceship spaceship;
    Asteroid asteroid;
    Planet planet1;
    Planet planet2;
    Planet planet3;
    Sun &sun;
};

#endif
