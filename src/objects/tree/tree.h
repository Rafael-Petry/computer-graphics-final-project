#ifndef TREE_H
#define TREE_H

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "../object.h"
#include "../../helpers/collision/colliders/boundingBox.h"

class Window;
class Planet;

class Tree : public Object
{
public:
    Tree(const glm::vec3 &color = glm::vec3(0.2f, 0.6f, 0.2f), Planet *planet = nullptr, const glm::vec3 &offset = glm::vec3(0.0f));

protected:
    void collide(Window *window) override;
    glm::mat4 translate(Window *window) override;
    glm::mat4 rotate(Window *window) override;

private:
    Planet *planet;
    glm::vec3 offset;
    static Mesh mesh;
    static BoundingBox boundingBox;
};

#endif
