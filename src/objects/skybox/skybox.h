#ifndef SKYBOX_H
#define SKYBOX_H

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "../../helpers/render/mesh.h"

class Window;

class Skybox
{
public:
    Skybox();
    static Skybox &getInstance();

    void update(GLint modelUniform, GLint colorUniform, GLint useTextureUniform, GLint texSamplerUniform, GLint isEmissiveUniform, Window *window);

private:
    static Mesh mesh;
    static bool textureGenerated;

    void generateStarTexture(int width, int height, unsigned int seed = 42);
};

#endif