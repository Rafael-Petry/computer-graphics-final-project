#ifndef SCENE_H
#define SCENE_H

#include "../objects/celestialBody/asteroid/asteroid.h"
#include "../objects/celestialBody/planet/planet.h"
#include "../objects/celestialBody/sun/sun.h"
#include "../objects/spaceship/spaceship.h"

class Window;

class Scene
{
public:
    Scene();
    void update(GLint modelUniform, GLint colorUniform, Window *window);

    Spaceship &getSpaceship();

private:
    float lastFrame;
    Spaceship spaceship;
    Asteroid asteroid;
    Planet planet;
    Sun &sun;
};

#endif
