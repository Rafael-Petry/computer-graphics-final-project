#ifndef SCENE_H
#define SCENE_H

#include "../objects/celestialBody/asteroid/asteroid.h"
#include "../objects/celestialBody/planet/planet.h"
#include "../objects/celestialBody/sun/sun.h"
#include "../objects/spaceship/spaceship.h"

class Scene
{
public:
    Scene();
    void update(GLint modelUniform, GLint colorUniform, float currentFrame) const;

private:
    Spaceship spaceship;
    Asteroid asteroid;
    Planet planet;
    Sun &sun;
};

#endif
