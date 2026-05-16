#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../objects/celestialBody/sun/sun.h"
#include "../window/window.h"
#include "scene.h"
#include "../../vendor/include/matrices.h"

Scene::Scene() : spaceship(), asteroid(), planet(), sun(Sun::getInstance()) { lastFrame = static_cast<float>(glfwGetTime()); };

void Scene::update(GLint modelUniform, GLint colorUniform, Window *window)
{
    spaceship.update(modelUniform, colorUniform, window);
    sun.update(modelUniform, colorUniform, window);
    planet.update(modelUniform, colorUniform, window);
    asteroid.update(modelUniform, colorUniform, window);
}
