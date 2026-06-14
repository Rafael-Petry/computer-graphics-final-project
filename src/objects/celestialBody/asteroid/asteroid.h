#ifndef ASTEROID_H
#define ASTEROID_H

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "../../objects/celestialBody/celestialBody.h"
#include "../../../helpers/collision/colliders/boundingSphere.h"

class Window;

class Asteroid : public CelestialBody
{
public:
    enum class Size
    {
        Small,
        Medium,
        Large
    };

    Asteroid(const glm::vec3 &color = glm::vec3(0.5f, 0.5f, 0.5f));
    void destroy();
    void destroyWithoutFragments();
    void setSize(Size newSize);
    Size getSize() const;
    bool isDestroyed() const;
    bool consumeFragmentSpawn(Size &outSize, glm::vec3 &outOrigin);

protected:
    glm::mat4 translate(Window *window) override;
    glm::mat4 rotate(Window *window) override;
    void collide(Window *window) override;

private:
    float chaseSpeed;
    float baseChaseSpeed = 2.8f;
    Size size = Size::Medium;
    float baseScale = 0.0085f;
    bool destroyed = false;
    bool pendingFragmentSpawn = false;
    Size fragmentSize = Size::Small;
    glm::vec3 fragmentOrigin = glm::vec3(0.0f);

    float rotateSpeedX = 0.0f;
    float rotateSpeedY = 0.0f;

    static Mesh mesh;
    static BoundingSphere boundingSphere;
};

#endif