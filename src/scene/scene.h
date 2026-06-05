#ifndef SCENE_H
#define SCENE_H

#include "../objects/celestialBody/asteroid/asteroid.h"
#include "../objects/celestialBody/planet/planet.h"
#include "../objects/celestialBody/sun/sun.h"
#include "../objects/spaceship/spaceship.h"

#include <list>
#include <vector>

class Window;

class Scene
{
public:
    Scene();
    void update(GLint modelUniform, GLint colorUniform, Window *window);

private:
    float lastFrame;
    Spaceship &spaceship;
    std::list<Asteroid> asteroids;
    std::vector<Planet> planets;
    Sun &sun;
};

#endif
