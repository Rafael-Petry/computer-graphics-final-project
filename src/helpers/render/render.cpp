#define TINYOBJLOADER_IMPLEMENTATION

#include <cmath>
#include <iostream>

#include <glm/vec3.hpp>

#include "mesh.h"
#include "render.h"

void RenderHelper::renderModel(GLint modelUniform, GLint colorUniform, const glm::mat4 &modelMatrix, const Mesh &mesh, const glm::vec3 &color)
{
    glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform3f(colorUniform, color.x, color.y, color.z);
    if (mesh.vao == 0 || mesh.vertexCount <= 0) {
        return;
    }

    glBindVertexArray(mesh.vao);
    glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
    glBindVertexArray(0);
}

Mesh RenderHelper::loadObjMesh(const std::string &path)
{
    Mesh mesh;

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
    mesh.attrib = attrib;
    mesh.shapes = shapes;
    return mesh;
}

Mesh RenderHelper::createCylinderMesh(int segments)
{
    Mesh mesh;

    if (segments < 3) {
        return mesh;
    }

    const float radius = 0.5f;
    const float height = 1.0f;
    const float halfHeight = height * 0.5f;
    const float step = 2.0f * static_cast<float>(M_PI) / static_cast<float>(segments);

    std::vector<float> positions;
    positions.reserve(static_cast<size_t>(segments) * 36);

    const glm::vec3 topCenter(0.0f, 0.0f, halfHeight);
    const glm::vec3 bottomCenter(0.0f, 0.0f, -halfHeight);

    for (int i = 0; i < segments; ++i) {
        const float angle0 = step * static_cast<float>(i);
        const float angle1 = step * static_cast<float>(i + 1);

        const glm::vec3 bottom0(radius * cosf(angle0), radius * sinf(angle0), -halfHeight);
        const glm::vec3 bottom1(radius * cosf(angle1), radius * sinf(angle1), -halfHeight);
        const glm::vec3 top0(radius * cosf(angle0), radius * sinf(angle0), halfHeight);
        const glm::vec3 top1(radius * cosf(angle1), radius * sinf(angle1), halfHeight);

        positions.insert(positions.end(), {bottom0.x, bottom0.y, bottom0.z, top0.x, top0.y, top0.z, top1.x, top1.y, top1.z});
        positions.insert(positions.end(), {bottom0.x, bottom0.y, bottom0.z, top1.x, top1.y, top1.z, bottom1.x, bottom1.y, bottom1.z});

        positions.insert(positions.end(), {topCenter.x, topCenter.y, topCenter.z, top0.x, top0.y, top0.z, top1.x, top1.y, top1.z});
        positions.insert(positions.end(), {bottomCenter.x, bottomCenter.y, bottomCenter.z, bottom1.x, bottom1.y, bottom1.z, bottom0.x, bottom0.y, bottom0.z});
    }

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);

    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * static_cast<GLsizei>(sizeof(float)), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    mesh.vertexCount = static_cast<GLsizei>(positions.size() / 3);
    return mesh;
}
