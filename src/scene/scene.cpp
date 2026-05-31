#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <algorithm>
#include <random>
#include <string>
#include <vector>

#include "../objects/celestialBody/sun/sun.h"
#include "../helpers/asteroidSpawner/asteroidSpawner.h"
#include "../window/window.h"
#include "scene.h"
#include "../../vendor/include/matrices.h"
#include <imgui.h>

Scene::Scene() : lastFrame(static_cast<float>(glfwGetTime())), spaceship(Spaceship::getInstance()), sun(Sun::getInstance())
{
    AsteroidSpawnerHelper::initialize(asteroids, spaceship);

    const std::vector<glm::vec3> colors = {
        glm::vec3(0.2f, 0.6f, 1.0f), glm::vec3(0.9f, 0.4f, 0.2f), glm::vec3(0.4f, 0.9f, 0.5f), glm::vec3(0.8f, 0.8f, 0.3f), glm::vec3(0.7f, 0.5f, 1.0f)};

    const float baseRadius = 50.0f;
    const float radiusStep = 50.0f;
    const int planetCount = 5;

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> phaseDist(0.0f, 2.0f);

    planets.reserve(planetCount);
    for (int i = 0; i < planetCount; ++i) {
        const float orbitRadius = baseRadius + (radiusStep * static_cast<float>(i));
        const float orbitSpeed = 0.01f;
        const float orbitPhase = phaseDist(rng);
        planets.emplace_back(colors[i % colors.size()], orbitRadius, orbitSpeed, orbitPhase);
    }
}

void Scene::update(GLint modelUniform, GLint colorUniform, Window *window)
{
    spaceship.update(modelUniform, colorUniform, window);
    sun.update(modelUniform, colorUniform, window);
    for (Planet &planet : planets) {
        planet.update(modelUniform, colorUniform, window);
    }
    for (Asteroid &asteroid : asteroids) {
        if (!asteroid.isDestroyed()) {
            asteroid.update(modelUniform, colorUniform, window);
        }
    }
    spaceship.updateShooting(modelUniform, colorUniform, window, asteroids);

    struct FragmentSpawn
    {
        Asteroid::Size size;
        glm::vec3 origin;
    };

    std::vector<FragmentSpawn> spawns;
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> offsetDist(-0.6f, 0.6f);
    for (Asteroid &asteroid : asteroids) {
        FragmentSpawn spawn;
        if (!asteroid.consumeFragmentSpawn(spawn.size, spawn.origin)) {
            continue;
        }
        spawns.push_back(spawn);
    }

    for (const FragmentSpawn &spawn : spawns) {
        for (int i = 0; i < 3; ++i) {
            asteroids.emplace_back();
            Asteroid &fragment = asteroids.back();
            fragment.setSize(spawn.size);
            glm::vec3 offset(offsetDist(rng), offsetDist(rng), offsetDist(rng));
            fragment.setPosition(spawn.origin + offset);
        }
    }

    asteroids.remove_if([](const Asteroid &asteroid) { return asteroid.isDestroyed(); });
    AsteroidSpawnerHelper::update(asteroids, spaceship, window->getCurrentFrame());

    ImGuiIO &io = ImGui::GetIO();
    const float padding = 12.0f;
    const ImGuiWindowFlags hudFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                                      ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize;

    ImGui::SetNextWindowPos(ImVec2(padding, padding), ImGuiCond_Always);
    ImGui::Begin("HUDLeft", nullptr, hudFlags);
    ImGui::Text("Health: %d", spaceship.getHealth());
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - padding, padding), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
    ImGui::Begin("HUDRight", nullptr, hudFlags);
    ImGui::Text("Score: %d", spaceship.getScore());
    ImGui::End();
}
