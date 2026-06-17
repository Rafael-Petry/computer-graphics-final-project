#ifndef SCENE_H
#define SCENE_H

#include "../objects/celestialBody/asteroid/asteroid.h"
#include "../objects/celestialBody/planet/planet.h"
#include "../objects/celestialBody/sun/sun.h"
#include "../objects/spaceship/spaceship.h"
#include "../objects/skybox/skybox.h"

#include <list>
#include <vector>

class Window;

class Scene
{
public:
    Scene();
    void update(GLint modelUniform,
                GLint colorUniform,
                GLint useTextureUniform,
                GLint texSamplerUniform,
                GLint isEmissiveUniform,
                GLint metallicUniform,
                GLint roughnessUniform,
                GLint specularUniform,
                Window *window);
    void updateUI(Window *window);
    void updateRadar(Window *window);

    static std::vector<Planet> &getPlanets() { return planets; }
    static std::list<Asteroid> &getAsteroids() { return asteroids; }

private:
    float lastFrame;
    Spaceship &spaceship;
    Sun &sun;
    Skybox &skybox;
    static std::list<Asteroid> asteroids;
    static std::vector<Planet> planets;
};

#endif