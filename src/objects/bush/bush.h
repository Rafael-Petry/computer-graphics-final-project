#ifndef BUSH_H
#define BUSH_H

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "../object.h"
#include "../../helpers/collision/colliders/boundingBox.h"

class Window;
class Planet;

class Bush : public Object
{
public:
    Bush(Planet *planet = nullptr, const glm::vec3 &offset = glm::vec3(0.0f));

protected:
    glm::mat4 translate(Window *window) override;
    glm::mat4 rotate(Window *window) override;

private:
    Planet *planet;
    glm::vec3 offset;
    static Mesh mesh;
    static BoundingBox boundingBox;
};

#endif
