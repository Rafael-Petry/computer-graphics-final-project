#ifndef ASTEROID_SPAWNER_H
#define ASTEROID_SPAWNER_H

#include <list>
#include <random>

#include <glm/vec3.hpp>

#include "../../objects/celestialBody/asteroid/asteroid.h"
#include "../../objects/spaceship/spaceship.h"

class AsteroidSpawnerHelper
{
public:
    static void initialize(std::list<Asteroid> &asteroids, const Spaceship &spaceship);
    static void update(std::list<Asteroid> &asteroids, const Spaceship &spaceship, float currentTime);
    static bool isWaitingForNextWave();
    static float getNextWaveStartTime();

private:
    static void spawnWave(std::list<Asteroid> &asteroids, const Spaceship &spaceship, int count);
    static glm::vec3 randomSpawnPosition(std::mt19937 &rng, const glm::vec3 &center);

    static bool initialized;
    static int currentWaveSize;
    static bool waitingForNextWave;
    static float nextWaveStartTime;

    static float minSpawnRadius;
    static float maxSpawnRadius;
    static float downtimeSeconds;
};

#endif
