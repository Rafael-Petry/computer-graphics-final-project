#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <string>

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include "../object.h"

class Window;

class Spaceship : public Object
{
public:
    Spaceship(const std::string &meshPath = "../../src/objects/spaceship/spaceship.obj", const glm::vec3 &color = glm::vec3(0.73f, 0.79f, 0.88f));

    void update(GLint modelUniform, GLint colorUniform, Window *window);

    glm::mat4 getViewMatrix() const;
    static void updateView(GLFWwindow *window, double xpos, double ypos);

protected:
    glm::mat4 translate(Window *window) override;
    glm::mat4 rotate(Window *window) override;
    glm::mat4 scale(Window *window) override;

private:
    void shoot() const;
    void updateOrientation();

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw = -90.0f;
    float pitch = 0.0f;

    float movementSpeed = 5.0f;

    float mouseSensitivity = 0.12f;
    bool firstMouseUpdate = true;
    float lastMouseX = 0.0f;
    float lastMouseY = 0.0f;
};

#endif
