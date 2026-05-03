#include "../objects/celestialBody/sun/sun.h"
#include "scene.h"
#include "../../vendor/include/matrices.h"

Scene::Scene() : spaceship(), asteroid(), planet(), sun(Sun::getInstance()) {};

void Scene::update(GLint modelUniform, GLint colorUniform, float currentFrame) const
{
    spaceship.update(modelUniform, colorUniform, currentFrame);
    sun.update(modelUniform, colorUniform, currentFrame);
    planet.update(modelUniform, colorUniform, currentFrame);
    asteroid.update(modelUniform, colorUniform, currentFrame);
}
