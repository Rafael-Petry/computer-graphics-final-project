#include <iostream>
#include <string>

#include "asteroid.h"
#include "../../spaceship/spaceship.h"
#include "../../../window/window.h"
#include "../../vendor/include/matrices.h"

Asteroid::Asteroid(const std::string &meshPath, const glm::vec3 &color) : CelestialBody(meshPath, color), position(2.7f, 0.4f, 0.0f, 1.0f) {}

glm::mat4 Asteroid::translate(Window *window)
{
    const glm::vec4 target = Spaceship::getInstance().getPosition();
    glm::vec4 direction = target - position;
    direction.w = 0.0f;

    const float distance = norm(direction);
    if (distance > 0.001f) {
        direction /= distance;
        position += direction * (chaseSpeed * window->getDeltaTime());
    }

    return Matrix_Translate(position.x, position.y, position.z);
}
glm::mat4 Asteroid::rotate(Window *window) { return Matrix_Rotate_Y(window->getCurrentFrame() * 1.7f); }
glm::mat4 Asteroid::scale(Window *window) { return Matrix_Scale(0.1f, 0.1f, 0.1f); }
