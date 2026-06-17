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
    // Rendu sans texture (fallback)
    static void renderModel(GLint modelUniform, GLint colorUniform, const glm::mat4 &modelMatrix, const Mesh &mesh, const glm::vec3 &color);

    // Rendu avec texture sur le VAO unique (planètes procédurales)
    static void renderModelTextured(
        GLint modelUniform, GLint colorUniform, GLint useTextureUniform, GLint textureSamplerUniform, const glm::mat4 &modelMatrix, const Mesh &mesh, const glm::vec3 &color);

    // Rendu complet avec support multi-matériaux (.mtl)
    // Gère automatiquement subMeshes si présents, sinon fallback sur VAO unique
    static void renderModelMTL(GLint modelUniform,
                               GLint colorUniform,
                               GLint useTextureUniform,
                               GLint textureSamplerUniform,
                               GLint metallicUniform,
                               GLint roughnessUniform,
                               GLint specularUniform,
                               GLint isEmissiveUniform,
                               const glm::mat4 &modelMatrix,
                               const Mesh &mesh,
                               const glm::vec3 &color);

    static Mesh loadObjMesh(const std::string &path);
    static Mesh createCylinderMesh(int segments);

    // Texture procédurale désertique pour les planètes
    static void generateDesertPlanetTexture(Mesh &mesh, int width = 512, int height = 512, unsigned int seed = 0);

    // Texture procédurale herbe pour les planètes
    static void generateGrassPlanetTexture(Mesh &mesh, int width = 512, int height = 512, unsigned int seed = 0);

    // Texture procédurale surface solaire
    static void generateSunTexture(Mesh &mesh, int width = 512, int height = 512);

private:
    // Charge une image .jpg/.png en texture GPU via stb_image
    // baseDir = dossier où chercher le fichier
    static GLuint loadTexture(const std::string &filename, const std::string &baseDir);
};

#endif