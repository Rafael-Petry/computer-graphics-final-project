#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../objects/celestialBody/sun/sun.h"
#include "scene.h"
#include "../../vendor/include/matrices.h"

Scene::Scene() : spaceship(), asteroid(), planet(), sun(Sun::getInstance()) { lastFrame = static_cast<float>(glfwGetTime()); };

void Scene::update(GLint modelUniform, GLint colorUniform, float currentFrame)
{
    float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    spaceship.update(modelUniform, colorUniform, deltaTime);
    sun.update(modelUniform, colorUniform, currentFrame);
    planet.update(modelUniform, colorUniform, currentFrame);
    asteroid.update(modelUniform, colorUniform, currentFrame);
}
