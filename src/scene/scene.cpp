#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <random>
#include <string>
#include <vector>

#include "../objects/celestialBody/sun/sun.h"
#include "../window/window.h"
#include "scene.h"
#include "../../vendor/include/matrices.h"
#include <imgui.h>

namespace {
    glm::vec3 randomAsteroidPosition(std::mt19937 &rng)
    {
        std::uniform_real_distribution<float> distX(-6.0f, 6.0f);
        std::uniform_real_distribution<float> distY(-1.0f, 1.0f);
        std::uniform_real_distribution<float> distZ(-6.0f, 6.0f);
        return glm::vec3(distX(rng), distY(rng), distZ(rng));
    }
}

Scene::Scene() : lastFrame(static_cast<float>(glfwGetTime())), spaceship(Spaceship::getInstance()), asteroidCount(10), planet(), sun(Sun::getInstance())
{
    std::mt19937 rng(std::random_device{}());
    for (int i = 0; i < asteroidCount; ++i) {
        asteroids.emplace_back();
        Asteroid &asteroid = asteroids.back();
        asteroid.setPosition(randomAsteroidPosition(rng));
    }
}

void Scene::update(GLint modelUniform, GLint colorUniform, Window *window)
{
    spaceship.update(modelUniform, colorUniform, window);
    sun.update(modelUniform, colorUniform, window);
    planet.update(modelUniform, colorUniform, window);
    for (Asteroid &asteroid : asteroids) {
        asteroid.update(modelUniform, colorUniform, window);
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
