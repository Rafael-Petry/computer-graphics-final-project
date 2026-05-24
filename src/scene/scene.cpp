#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../objects/celestialBody/sun/sun.h"
#include "../window/window.h"
#include "scene.h"
#include "../../vendor/include/matrices.h"

Scene::Scene() : lastFrame(static_cast<float>(glfwGetTime())), spaceship(Spaceship::getInstance()), asteroid(), planet(), sun(Sun::getInstance()), collisionGrid(1.0f) {}

void Scene::update(GLint modelUniform, GLint colorUniform, Window *window)
{
    spaceship.update(modelUniform, colorUniform, window);
    sun.update(modelUniform, colorUniform, window);
    planet.update(modelUniform, colorUniform, window);
    asteroid.update(modelUniform, colorUniform, window);

    checkCollisions();
}

void Scene::checkCollisions()
{
    // I was afraid that when we add trees to each planet, the amount of colliders added to the grid would increase drastically
    // But, an easy workaround that could be tested is, since the trees always have the same position relative to the planet,
    // we can check the distance from spaceship to the planet, and only add the planet's trees to the grid if the spaceship is
    // close enough to the planet.

    collisionGrid.clear();

    const glm::vec3 spaceshipPosition(spaceship.getPosition());
    const glm::vec3 spaceshipScale(0.3f);
    collisionGrid.addBoundingBox(spaceship.getBoundingBox(), spaceshipPosition, spaceshipScale);

    const glm::vec3 asteroidPosition(asteroid.getPosition());
    const glm::vec3 asteroidScale(0.1f);
    collisionGrid.addBoundingSphere(asteroid.getBoundingSphere(), asteroidPosition, asteroidScale);

    // I think in order to run different functionalities when colliding with different objects I could
    // add a pointer to the object in the CollisionProxy struct, and then run a specific collide function
    collisionGrid.forEachPotentialPair([&](const CollisionProxy &a, const CollisionProxy &b) {
        if (a.shape == ColliderShape::Sphere && b.shape == ColliderShape::Box) {
            a.sphere->testCollision(*b.box, a.position, a.scale, b.position, b.scale);
            return;
        }

        if (a.shape == ColliderShape::Box && b.shape == ColliderShape::Sphere) {
            b.sphere->testCollision(*a.box, b.position, b.scale, a.position, a.scale);
            return;
        }

        if (a.shape == ColliderShape::Sphere && b.shape == ColliderShape::Sphere) {
            a.sphere->testCollision(*b.sphere, a.position, a.scale, b.position, b.scale);
            return;
        }

        a.box->testCollision(*b.box, a.position, a.scale, b.position, b.scale);
    });
}
