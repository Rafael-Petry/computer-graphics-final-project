#ifndef RENDER_H
#define RENDER_H

#include <string>
#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.h"

class RenderHelper
{
public:
    // Rendu sans texture (comportement existant)
    static void renderModel(GLint modelUniform, GLint colorUniform, const glm::mat4 &modelMatrix, const Mesh &mesh, const glm::vec3 &color);

    // Rendu avec texture (passe useTexture=true et bind la texture du mesh)
    static void renderModelTextured(
        GLint modelUniform, GLint colorUniform, GLint useTextureUniform, GLint textureSamplerUniform, const glm::mat4 &modelMatrix, const Mesh &mesh, const glm::vec3 &color);

    static Mesh loadObjMesh(const std::string &path);
    static Mesh createCylinderMesh(int segments);

    // Génère une texture procédurale désertique/rocheuse et la stocke dans mesh.textureId
    static void generateDesertPlanetTexture(Mesh &mesh, int width = 512, int height = 512, unsigned int seed = 0);
};

#endif