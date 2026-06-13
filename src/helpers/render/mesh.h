#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include <glad/glad.h>

#include "../../vendor/include/tiny_obj_loader.h"

// Un sous-mesh correspond à un groupe de faces partageant le même matériau
struct SubMesh
{
    GLuint vao = 0;
    GLuint vbo = 0;
    GLsizei vertexCount = 0;

    // Texture diffuse (map_Kd)
    GLuint textureId = 0; // 0 = pas de texture

    // Propriétés PBR extraites du .mtl
    float metallic = 0.0f;
    float roughness = 0.6f;
    float specular = 0.5f;
    bool isEmissive = false; // Ke > 0
};

struct Mesh
{
    // Sous-meshes par matériau (remplis par loadObjMesh si .mtl présent)
    std::vector<SubMesh> subMeshes;

    // Fallback : VAO unique (utilisé si pas de .mtl ou pour meshes procéduraux)
    GLuint vao = 0;
    GLuint vbo = 0;
    GLsizei vertexCount = 0;
    GLuint textureId = 0; // texture procédurale (planètes)

    // Données brutes pour la collision
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;

    bool hasSubMeshes() const { return !subMeshes.empty(); }
};

#endif