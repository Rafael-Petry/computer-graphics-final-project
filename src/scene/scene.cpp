#include "scene.h"
#include "../vendor/include/matrices.h"

Scene::Scene() : spaceship(), asteroid(), planet1(), planet2(), planet3(), sun(Sun::getInstance()) {};

void Scene::update(GLint modelUniform, GLint colorUniform, float currentFrame) const
{
    const glm::mat4 sunModel = Matrix_Scale(1.1f, 1.1f, 1.1f);
    sun.update(modelUniform, colorUniform, sunModel);

    const glm::mat4 spaceshipModel = Matrix_Translate(0.0f, 0.2f, 5.0f) * Matrix_Rotate_Y(currentFrame);
    spaceship.update(modelUniform, colorUniform, spaceshipModel);

    const glm::mat4 asteroidModel =
        Matrix_Translate(2.7f * cosf(currentFrame * 0.8f), 0.4f, 2.7f * sinf(currentFrame * 0.8f)) * Matrix_Rotate_Y(currentFrame * 1.7f) * Matrix_Scale(0.35f, 0.35f, 0.35f);
    asteroid.update(modelUniform, colorUniform, asteroidModel);

    const glm::mat4 planet1Model = Matrix_Translate(2.0f * cosf(currentFrame * 0.4f), 0.0f, 2.0f * sinf(currentFrame * 0.4f)) * Matrix_Scale(0.45f, 0.45f, 0.45f);
    planet1.update(modelUniform, colorUniform, planet1Model);

    const glm::mat4 planet2Model = Matrix_Translate(3.3f * cosf(currentFrame * 0.25f), -0.1f, 3.3f * sinf(currentFrame * 0.25f)) * Matrix_Scale(0.6f, 0.6f, 0.6f);
    planet2.update(modelUniform, colorUniform, planet2Model);

    const glm::mat4 planet3Model = Matrix_Translate(4.6f * cosf(currentFrame * 0.18f), 0.15f, 4.6f * sinf(currentFrame * 0.18f)) * Matrix_Scale(0.35f, 0.35f, 0.35f);
    planet3.update(modelUniform, colorUniform, planet3Model);
}