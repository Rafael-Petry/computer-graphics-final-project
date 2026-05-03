#define TINYOBJLOADER_IMPLEMENTATION

#include "render.h"

#include <iostream>
#include <unordered_map>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

#include "../../vendor/include/tiny_obj_loader.h"

namespace {
    struct MeshData
    {
        GLuint vao = 0;
        GLuint vbo = 0;
        GLsizei vertexCount = 0;
    };

    MeshData loadObjMesh(const std::string &path)
    {
        MeshData mesh;

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn;
        std::string err;

        const bool loaded = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), nullptr, true);

        if (!warn.empty()) {
            std::cerr << "OBJ load warning: " << warn << std::endl;
        }

        if (!err.empty()) {
            std::cerr << "OBJ load error: " << err << std::endl;
        }

        if (!loaded) {
            std::cerr << "Failed to load OBJ file: " << path << std::endl;
            return mesh;
        }

        std::vector<float> trianglePositions;
        trianglePositions.reserve(shapes.size() * 9);

        for (const auto &shape : shapes) {
            for (const auto &index : shape.mesh.indices) {
                const int vertexIndex = index.vertex_index;
                if (vertexIndex < 0) {
                    continue;
                }

                const size_t base = static_cast<size_t>(vertexIndex) * 3;
                if (base + 2 >= attrib.vertices.size()) {
                    continue;
                }

                trianglePositions.push_back(attrib.vertices[base]);
                trianglePositions.push_back(attrib.vertices[base + 1]);
                trianglePositions.push_back(attrib.vertices[base + 2]);
            }
        }

        if (trianglePositions.empty()) {
            std::cerr << "No triangles parsed from OBJ file: " << path << std::endl;
            return mesh;
        }

        glGenVertexArrays(1, &mesh.vao);
        glGenBuffers(1, &mesh.vbo);

        glBindVertexArray(mesh.vao);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glBufferData(GL_ARRAY_BUFFER, trianglePositions.size() * sizeof(float), trianglePositions.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * static_cast<GLsizei>(sizeof(float)), reinterpret_cast<void *>(0));
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);

        mesh.vertexCount = static_cast<GLsizei>(trianglePositions.size() / 3);
        return mesh;
    }

    const MeshData &getMesh(const std::string &path)
    {
        static std::unordered_map<std::string, MeshData> cache;
        const auto it = cache.find(path);
        if (it != cache.end()) {
            return it->second;
        }

        const MeshData mesh = loadObjMesh(path);
        const auto inserted = cache.emplace(path, mesh);
        return inserted.first->second;
    }
}

void RenderHelper::renderModel(GLint modelUniform, GLint colorUniform, const glm::mat4 &modelMatrix, const std::string &modelPath, const glm::vec3 &color)
{
    glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform3f(colorUniform, color.x, color.y, color.z);

    const MeshData &mesh = getMesh(modelPath);
    if (mesh.vao == 0 || mesh.vertexCount <= 0) {
        return;
    }

    glBindVertexArray(mesh.vao);
    glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
    glBindVertexArray(0);
}
