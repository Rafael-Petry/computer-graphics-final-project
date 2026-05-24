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
    collisionGrid.clear();

    const glm::vec3 spaceshipPosition(spaceship.getPosition());
    const glm::vec3 spaceshipScale(0.3f);
    collisionGrid.addBoundingBox(spaceship.getBoundingBox(), spaceshipPosition, spaceshipScale);

    const glm::vec3 asteroidPosition(asteroid.getPosition());
    const glm::vec3 asteroidScale(0.1f);
    collisionGrid.addBoundingSphere(asteroid.getBoundingSphere(), asteroidPosition, asteroidScale);

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
