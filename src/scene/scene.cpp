#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <algorithm>
#include <cmath>
#include <random>
#include <string>
#include <vector>

#include "../objects/celestialBody/sun/sun.h"
#include "../helpers/collision/colliders/boundingSphere.h"
#include "../helpers/spawners/asteroid/asteroidSpawner.h"
#include "../window/window.h"
#include "scene.h"
#include "../../vendor/include/matrices.h"
#include <imgui.h>

namespace {
    ImVec2 projectToRadar(const glm::vec3 &delta, float radarRange, float radarHalf)
    {
        float x = delta.x;
        float y = -delta.z;
        float maxAbs = std::max(std::fabs(x), std::fabs(y));
        if (maxAbs > radarRange && maxAbs > 0.0f) {
            const float scale = radarRange / maxAbs;
            x *= scale;
            y *= scale;
        }

        const float pixelScale = radarHalf / radarRange;
        return ImVec2(x * pixelScale, y * pixelScale);
    }

    void drawRadarMarker(ImDrawList *drawList, const ImVec2 &position, ImU32 color, float size, int verticalDirection)
    {
        if (verticalDirection > 0) {
            const ImVec2 top(position.x, position.y - size);
            const ImVec2 left(position.x - size * 0.6f, position.y + size);
            const ImVec2 right(position.x + size * 0.6f, position.y + size);
            drawList->AddTriangleFilled(top, left, right, color);
            return;
        }

        if (verticalDirection < 0) {
            const ImVec2 bottom(position.x, position.y + size);
            const ImVec2 left(position.x - size * 0.6f, position.y - size);
            const ImVec2 right(position.x + size * 0.6f, position.y - size);
            drawList->AddTriangleFilled(bottom, left, right, color);
            return;
        }

        drawList->AddCircleFilled(position, size * 0.6f, color);
    }
}

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
    std::uniform_real_distribution<float> treeBushDist(0.0f, 1.0f);

    planets.reserve(planetCount);
    for (int i = 0; i < planetCount; ++i) {
        const float orbitRadius = baseRadius + (radiusStep * static_cast<float>(i));
        const float orbitSpeed = 0.01f;
        const float orbitPhase = phaseDist(rng);
        planets.emplace_back(colors[i % colors.size()], orbitRadius, orbitSpeed, orbitPhase);
        Planet &currentPlanet = planets[i];

        // Spawn trees and bushes on the planet's surface
        const int treesPerPlanet = 3;
        const int bushesPerPlanet = 5;
        const float planetRadius = 15.0f;                // Approximate planet scale
        const float spawnDistance = planetRadius + 1.5f; // Distance from planet center to surface spawn point

        for (int t = 0; t < treesPerPlanet; ++t) {
            float theta = (2.0f * 3.14159f * t) / treesPerPlanet;
            float phi = (3.14159f * treeBushDist(rng));

            glm::vec3 offset(spawnDistance * std::sin(phi) * std::cos(theta), spawnDistance * std::cos(phi), spawnDistance * std::sin(phi) * std::sin(theta));
            glm::vec3 treeColor = colors[i % colors.size()] * glm::vec3(0.5f, 1.0f, 0.5f);
            trees.emplace_back(treeColor, &currentPlanet, offset, 0.8f);
        }

        for (int b = 0; b < bushesPerPlanet; ++b) {
            float theta = (2.0f * 3.14159f * b) / bushesPerPlanet + (3.14159f / bushesPerPlanet) * 0.5f;
            float phi = (3.14159f * treeBushDist(rng));

            glm::vec3 offset(spawnDistance * std::sin(phi) * std::cos(theta), spawnDistance * std::cos(phi), spawnDistance * std::sin(phi) * std::sin(theta));
            glm::vec3 bushColor = colors[i % colors.size()] * glm::vec3(0.6f, 1.0f, 0.6f);
            bushes.emplace_back(bushColor, &currentPlanet, offset, 0.6f);
        }
    }
}

void Scene::update(GLint modelUniform,
                   GLint colorUniform,
                   GLint useTextureUniform,
                   GLint texSamplerUniform,
                   GLint isEmissiveUniform,
                   GLint metallicUniform,
                   GLint roughnessUniform,
                   GLint specularUniform,
                   Window *window)
{
    spaceship.update(modelUniform, colorUniform, window);
    sun.update(modelUniform, colorUniform, window, useTextureUniform, texSamplerUniform, isEmissiveUniform, true, metallicUniform, roughnessUniform, specularUniform);

    for (Planet &planet : planets) {
        planet.update(modelUniform, colorUniform, window, useTextureUniform, texSamplerUniform, isEmissiveUniform, false, metallicUniform, roughnessUniform, specularUniform);
    }

    for (Tree &tree : trees) {
        tree.update(modelUniform, colorUniform, window, useTextureUniform, texSamplerUniform, isEmissiveUniform, false, metallicUniform, roughnessUniform, specularUniform);
    }

    for (Bush &bush : bushes) {
        bush.update(modelUniform, colorUniform, window, useTextureUniform, texSamplerUniform, isEmissiveUniform, false, metallicUniform, roughnessUniform, specularUniform);
    }

    for (Asteroid &asteroid : asteroids) {
        if (!asteroid.isDestroyed()) {
            asteroid.update(modelUniform, colorUniform, window, useTextureUniform, texSamplerUniform, isEmissiveUniform, false, metallicUniform, roughnessUniform, specularUniform);
        }
    }

    for (Asteroid &asteroid : asteroids) {
        if (asteroid.isDestroyed()) {
            continue;
        }

        const BoundingSphere *sphere = dynamic_cast<const BoundingSphere *>(&asteroid.getCollider());
        if (sphere == nullptr) {
            continue;
        }

        if (sphere->testCollisionBoundingSphere(asteroid, sun)) {
            asteroid.destroyWithoutFragments();
            continue;
        }

        for (Planet &planet : planets) {
            if (sphere->testCollisionBoundingSphere(asteroid, planet)) {
                asteroid.destroyWithoutFragments();
                break;
            }
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
    std::uniform_real_distribution<float> offsetDist(-12.0f, 12.0f);
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

    this->updateUI(window);
}

void Scene::updateUI(Window *window)
{
    ImGuiIO &io = ImGui::GetIO();
    const float padding = 12.0f;
    const ImGuiWindowFlags hudFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                                      ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize;

    ImGui::SetNextWindowPos(ImVec2(padding, padding), ImGuiCond_Always);
    ImGui::Begin("HUDLeft", nullptr, hudFlags);
    ImGui::Text("Health: %d", spaceship.getHealth());
    ImGui::End();

    if (AsteroidSpawnerHelper::isWaitingForNextWave()) {
        const float secondsLeft = std::max(0.0f, AsteroidSpawnerHelper::getNextWaveStartTime() - window->getCurrentFrame());
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, padding), ImGuiCond_Always, ImVec2(0.5f, 0.0f));
        ImGui::Begin("WaveCountdown", nullptr, hudFlags);
        ImGui::Text("Next wave in: %.1f", secondsLeft);
        ImGui::End();
    }

    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - padding, padding), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
    ImGui::Begin("HUDRight", nullptr, hudFlags);
    ImGui::Text("Score: %d", spaceship.getScore());
    ImGui::End();

    this->updateRadar(window);
}

void Scene::updateRadar(Window *window)
{
    ImGuiIO &io = ImGui::GetIO();
    const float padding = 12.0f;

    const float radarSize = 320.0f;
    const float radarRange = 120.0f;
    const float radarHalf = radarSize * 0.5f;
    const float markerSize = 6.0f;
    const float verticalThreshold = 0.5f;
    const ImU32 radarBackground = IM_COL32(6, 8, 18, 200);
    const ImU32 radarBorder = IM_COL32(220, 220, 220, 220);
    const ImU32 shipColor = IM_COL32(255, 255, 255, 255);
    const ImU32 asteroidColor = IM_COL32(235, 80, 80, 255);
    const ImU32 planetColor = IM_COL32(80, 230, 80, 255);
    const ImU32 sunColor = IM_COL32(245, 220, 80, 255);

    const ImGuiWindowFlags radarFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                                        ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground;

    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - padding, io.DisplaySize.y - padding), ImGuiCond_Always, ImVec2(1.0f, 1.0f));
    ImGui::SetNextWindowSize(ImVec2(radarSize, radarSize), ImGuiCond_Always);
    ImGui::Begin("Radar", nullptr, radarFlags);

    ImDrawList *drawList = ImGui::GetWindowDrawList();
    const ImVec2 radarPos = ImGui::GetWindowPos();
    const ImVec2 radarMax = ImVec2(radarPos.x + radarSize, radarPos.y + radarSize);
    const ImVec2 radarCenter = ImVec2(radarPos.x + radarHalf, radarPos.y + radarHalf);

    drawList->AddRectFilled(radarPos, radarMax, radarBackground, 3.0f);
    drawList->AddRect(radarPos, radarMax, radarBorder, 3.0f);

    const glm::vec3 shipPosition = spaceship.getPosition();
    glm::vec3 shipFront = spaceship.getFrontVector();
    shipFront.y = 0.0f;
    float shipFrontLength = glm::length(shipFront);
    if (shipFrontLength > 0.0001f) {
        shipFront /= shipFrontLength;
    } else {
        shipFront = glm::vec3(0.0f, 0.0f, -1.0f);
    }

    const ImVec2 shipDir = ImVec2(shipFront.x, -shipFront.z);
    const float shipArrowSize = markerSize * 1.2f;
    const ImVec2 shipTip = ImVec2(radarCenter.x + shipDir.x * shipArrowSize, radarCenter.y + shipDir.y * shipArrowSize);
    const ImVec2 shipLeft = ImVec2(radarCenter.x - shipDir.y * shipArrowSize * 0.6f - shipDir.x * shipArrowSize * 0.4f,
                                   radarCenter.y + shipDir.x * shipArrowSize * 0.6f - shipDir.y * shipArrowSize * 0.4f);
    const ImVec2 shipRight = ImVec2(radarCenter.x + shipDir.y * shipArrowSize * 0.6f - shipDir.x * shipArrowSize * 0.4f,
                                    radarCenter.y - shipDir.x * shipArrowSize * 0.6f - shipDir.y * shipArrowSize * 0.4f);
    drawList->AddTriangleFilled(shipTip, shipLeft, shipRight, shipColor);

    auto drawRadarObject = [&](const glm::vec3 &position, ImU32 color) {
        const glm::vec3 delta = position - shipPosition;
        const ImVec2 offset = projectToRadar(delta, radarRange, radarHalf);
        const ImVec2 markerPos = ImVec2(radarCenter.x + offset.x, radarCenter.y + offset.y);
        int verticalDirection = 0;
        if (delta.y > verticalThreshold) {
            verticalDirection = 1;
        } else if (delta.y < -verticalThreshold) {
            verticalDirection = -1;
        }
        drawRadarMarker(drawList, markerPos, color, markerSize, verticalDirection);
    };

    for (const Asteroid &asteroid : asteroids) {
        if (!asteroid.isDestroyed()) {
            drawRadarObject(asteroid.getPosition(), asteroidColor);
        }
    }

    for (const Planet &planet : planets) {
        drawRadarObject(planet.getPosition(), planetColor);
    }

    drawRadarObject(sun.getPosition(), sunColor);

    ImGui::End();
}