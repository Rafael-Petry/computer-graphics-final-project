#include <glm/geometric.hpp>

#include "asteroidSpawner.h"

bool AsteroidSpawnerHelper::initialized = false;
int AsteroidSpawnerHelper::currentWaveSize = 10;
bool AsteroidSpawnerHelper::waitingForNextWave = false;
float AsteroidSpawnerHelper::nextWaveStartTime = 0.0f;

float AsteroidSpawnerHelper::minSpawnRadius = 60.0f;
float AsteroidSpawnerHelper::maxSpawnRadius = 80.0f;
float AsteroidSpawnerHelper::downtimeSeconds = 30.0f;

void AsteroidSpawnerHelper::initialize(std::list<Asteroid> &asteroids, const Spaceship &spaceship)
{
    currentWaveSize = 5;
    waitingForNextWave = false;
    nextWaveStartTime = 0.0f;
    initialized = true;

    spawnWave(asteroids, spaceship, currentWaveSize);
}

void AsteroidSpawnerHelper::update(std::list<Asteroid> &asteroids, const Spaceship &spaceship, float currentTime)
{
    if (!initialized) {
        initialize(asteroids, spaceship);
        return;
    }

    int aliveCount = 0;
    for (const Asteroid &asteroid : asteroids) {
        if (!asteroid.isDestroyed()) {
            ++aliveCount;
        }
    }

    if (aliveCount > 0) {
        return;
    }

    if (!waitingForNextWave) {
        waitingForNextWave = true;
        nextWaveStartTime = currentTime + downtimeSeconds;
        return;
    }

    if (currentTime < nextWaveStartTime) {
        return;
    }

    currentWaveSize += 5;
    spawnWave(asteroids, spaceship, currentWaveSize);
    waitingForNextWave = false;
}

void AsteroidSpawnerHelper::spawnWave(std::list<Asteroid> &asteroids, const Spaceship &spaceship, int count)
{
    std::mt19937 rng(std::random_device{}());
    const glm::vec3 center = spaceship.getPosition();

    for (int i = 0; i < count; ++i) {
        asteroids.emplace_back();
        Asteroid &asteroid = asteroids.back();
        asteroid.setPosition(randomSpawnPosition(rng, center));
    }
}

bool AsteroidSpawnerHelper::isWaitingForNextWave() { return waitingForNextWave; }

float AsteroidSpawnerHelper::getNextWaveStartTime() { return nextWaveStartTime; }

glm::vec3 AsteroidSpawnerHelper::randomSpawnPosition(std::mt19937 &rng, const glm::vec3 &center)
{
    std::uniform_real_distribution<float> directionDist(-1.0f, 1.0f);
    std::uniform_real_distribution<float> radiusDist(minSpawnRadius, maxSpawnRadius);

    glm::vec3 direction(0.0f);
    float directionLength = 0.0f;

    while (directionLength < 0.001f) {
        direction = glm::vec3(directionDist(rng), directionDist(rng), directionDist(rng));
        directionLength = glm::length(direction);
    }

    direction /= directionLength;
    const float radius = radiusDist(rng);
    return center + direction * radius;
}
