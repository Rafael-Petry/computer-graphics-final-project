#ifndef SCENE_H
#define SCENE_H

#include "../objects/celestialBody/asteroid/asteroid.h"
#include "../objects/celestialBody/planet/planet.h"
#include "../objects/celestialBody/sun/sun.h"
#include "../objects/spaceship/spaceship.h"
#include "../helpers/collision/uniformGrid.h"

class Window;

class Scene
{
public:
    Scene();
    void update(GLint modelUniform, GLint colorUniform, Window *window);

private:
    void checkCollisions();

    float lastFrame;
    Spaceship &spaceship;
    Asteroid asteroid;
    Planet planet;
    Sun &sun;
    UniformGridBroadPhase collisionGrid;
};

#endif
