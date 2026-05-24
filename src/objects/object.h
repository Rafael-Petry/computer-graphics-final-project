#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "../helpers/render/mesh.h"

class Collider;

class Window;

class Object
{
public:
    Object(const Mesh &mesh, const Collider &collider, const glm::vec3 &color = glm::vec3(0.5f, 0.5f, 0.5f));
    virtual ~Object() {}

    void update(GLint modelUniform, GLint colorUniform, Window *window);
    const Mesh &getMesh() const;
    const Collider &getCollider() const;
    const glm::vec3 &getScale() const;
    const glm::vec3 &getPosition() const;

protected:
    const Mesh &mesh;
    const Collider &collider;
    const glm::vec3 color;
    glm::vec3 scaleValue;
    glm::vec3 positionValue;

    virtual glm::mat4 translate(Window *window) = 0;
    virtual glm::mat4 rotate(Window *window) = 0;
    virtual glm::mat4 scale(Window *window);
    virtual void collide(Window *window);
};

#endif
