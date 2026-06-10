#include <iostream>
#include <random>
#include <glm/geometric.hpp>
#include "asteroid.h"
#include "../../../helpers/collision/collision.h"
#include "../../../helpers/render/render.h"
#include "../../spaceship/spaceship.h"
#include "../../../window/window.h"
#include "../../vendor/include/matrices.h"

Mesh Asteroid::mesh;
BoundingSphere Asteroid::boundingSphere;

namespace {
    float sizeScale(Asteroid::Size size)
    {
        switch (size) {
        case Asteroid::Size::Small:
            return 1.0f;
        case Asteroid::Size::Large:
            return 5.0f;
        case Asteroid::Size::Medium:
        default:
            return 3.0f;
        }
    }

    float sizeSpeedScale(Asteroid::Size size)
    {
        switch (size) {
        case Asteroid::Size::Small:
            return 1.3f;
        case Asteroid::Size::Large:
            return 0.8f;
        case Asteroid::Size::Medium:
        default:
            return 1.0f;
        }
    }

    Asteroid::Size randomAsteroidSize(std::mt19937 &rng)
    {
        std::uniform_int_distribution<int> dist(0, 2);
        switch (dist(rng)) {
        case 0:
            return Asteroid::Size::Small;
        case 2:
            return Asteroid::Size::Large;
        case 1:
        default:
            return Asteroid::Size::Medium;
        }
    }
}

Asteroid::Asteroid(const glm::vec3 &color) : CelestialBody(mesh, boundingSphere, color)
{
    if (mesh.vao == 0) {
        mesh = RenderHelper::loadObjMesh("../../src/objects/celestialBody/asteroid/asteroid_updated.obj");
    }

    if (!boundingSphere.isInitialized() && mesh.vao != 0) {
        boundingSphere = CollisionHelper::generateBoundingSphere(mesh);
    }

    std::mt19937 rng(std::random_device{}());
    setSize(randomAsteroidSize(rng));
    position = glm::vec3(2.7f, 0.4f, 0.0f);
}

void Asteroid::setSize(Size newSize)
{
    size = newSize;
    scaleValue = glm::vec3(baseScale * sizeScale(size));
    chaseSpeed = baseChaseSpeed * sizeSpeedScale(size);
}

Asteroid::Size Asteroid::getSize() const { return size; }

bool Asteroid::isDestroyed() const { return destroyed; }

bool Asteroid::consumeFragmentSpawn(Size &outSize, glm::vec3 &outOrigin)
{
    if (!pendingFragmentSpawn) {
        return false;
    }

    pendingFragmentSpawn = false;
    outSize = fragmentSize;
    outOrigin = fragmentOrigin;
    return true;
}

glm::mat4 Asteroid::translate(Window *window)
{
    const glm::vec3 target = Spaceship::getInstance().getPosition();
    glm::vec3 direction = target - position;

    const float distance = glm::length(direction);
    if (distance > 0.001f) {
        direction /= distance;
        position += direction * (chaseSpeed * window->getDeltaTime());
    }

    return Matrix_Translate(position.x, position.y, position.z);
}
glm::mat4 Asteroid::rotate(Window *window) { return Matrix_Rotate_Y(window->getCurrentFrame() * 1.7f); }

void Asteroid::collide(Window *window)
{
    const Spaceship &spaceship = Spaceship::getInstance();

    if (boundingSphere.testCollisionBoundingBox(*this, spaceship)) {
        Spaceship::getInstance().applyDamage((int)size + 1);
        destroyWithoutFragments();
    }
}

void Asteroid::destroy()
{
    if (size != Size::Small) {
        pendingFragmentSpawn = true;
        fragmentSize = (size == Size::Large) ? Size::Medium : Size::Small;
        fragmentOrigin = position;
    }

    destroyed = true;
    scaleValue = glm::vec3(0.0f);
    chaseSpeed = 0.0f;
}

void Asteroid::destroyWithoutFragments()
{
    pendingFragmentSpawn = false;
    destroyed = true;
    scaleValue = glm::vec3(0.0f);
    chaseSpeed = 0.0f;
}
