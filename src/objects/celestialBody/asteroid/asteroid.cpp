#include <iostream>
#include <random>
#include <glm/geometric.hpp>
#include "asteroid.h"
#include "../../../helpers/collision/collision.h"
#include "../../../helpers/render/render.h"
#include "../../spaceship/spaceship.h"
#include "../../../window/window.h"
#include "../../vendor/include/matrices.h"
#include "../../objects/celestialBody/sun/sun.h"
#include "../../../scene/scene.h"

Mesh Asteroid::mesh;
BoundingSphere Asteroid::boundingSphere;

namespace {
    float sizeScale(Asteroid::Size size)
    {
        switch (size) {
        case Asteroid::Size::Small:
            return 0.2f;
        case Asteroid::Size::Large:
            return 1.2f;
        case Asteroid::Size::Medium:
        default:
            return 0.8f;
        }
    }

    float sizeSpeedScale(Asteroid::Size size)
    {
        switch (size) {
        case Asteroid::Size::Small:
            return 7.6f;
        case Asteroid::Size::Large:
            return 6.8f;
        case Asteroid::Size::Medium:
        default:
            return 7.1f;
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

Asteroid::Asteroid() : CelestialBody(mesh, boundingSphere, color)
{
    if (mesh.vao == 0 && !mesh.hasSubMeshes()) {
        mesh = RenderHelper::loadObjMesh("../../src/objects/celestialBody/asteroid/asteroid.obj");
    }

    if (!boundingSphere.isInitialized() && (mesh.vao != 0 || mesh.hasSubMeshes())) {
        boundingSphere = CollisionHelper::generateBoundingSphere(mesh);
    }

    std::mt19937 rng(std::random_device{}());
    setSize(randomAsteroidSize(rng));
    position = glm::vec3(2.7f, 0.4f, 0.0f);

    static std::mt19937 genX(std::random_device{}());
    static std::mt19937 genY(std::random_device{}());
    std::uniform_real_distribution<float> rotateSpeed(-1.7f, 1.7f);

    rotateSpeedX = rotateSpeed(genX);
    rotateSpeedY = rotateSpeed(genY);
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

glm::mat4 Asteroid::rotate(Window *window) { return Matrix_Rotate_Y(window->getCurrentFrame() * rotateSpeedY) * Matrix_Rotate_X(window->getCurrentFrame() * rotateSpeedX); }

void Asteroid::collide(Window *window)
{
    const Spaceship &spaceship = Spaceship::getInstance();

    if (boundingSphere.testCollisionBoundingSphere(*this, Sun::getInstance())) {
        destroy(false);
        return;
    }

    if (boundingSphere.testCollisionBoundingBox(*this, spaceship)) {
        Spaceship::getInstance().applyDamage((int)size + 1);
        destroy(false);
    }

    for (Planet &planet : Scene::getPlanets()) {
        if (boundingSphere.testCollisionBoundingSphere(*this, planet)) {
            destroy(false);
            break;
        }
    }
}

void Asteroid::destroy(bool spawnFragments)
{
    if (spawnFragments && size != Size::Small) {
        std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<float> offsetDist(-12.0f, 12.0f);

        for (int i = 0; i < 2; i++) {
            Scene::getAsteroids().emplace_back();
            glm::vec3 offset(offsetDist(rng), offsetDist(rng), offsetDist(rng));

            Asteroid &fragment = Scene::getAsteroids().back();
            fragment.setSize((size == Size::Large) ? Size::Medium : Size::Small);
            fragment.setPosition(position + offset);
        }

        fragmentSize = (size == Size::Large) ? Size::Medium : Size::Small;
        fragmentOrigin = position;
    }

    destroyed = true;
}
