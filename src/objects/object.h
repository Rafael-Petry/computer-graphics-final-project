#ifndef OBJECT_H
#define OBJECT_H

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Window;

class Object
{
public:
    Object(std::string meshPath = "../../src/objects/celestialBody/celestialBody.obj", glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f));
    virtual ~Object() {}

    void update(GLint modelUniform, GLint colorUniform, Window *window);

protected:
    const std::string meshPath;
    const glm::vec3 color;

    virtual glm::mat4 translate(Window *window) = 0;
    virtual glm::mat4 rotate(Window *window) = 0;
    virtual glm::mat4 scale(Window *window) = 0;
};

#endif
