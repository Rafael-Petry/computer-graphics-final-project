#include <cmath>
#include <limits>

#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include "collision.h"

BoundingBox CollisionHelper::generateBoundingBox(const Mesh &mesh)
{
    if (mesh.attrib.vertices.empty() || mesh.shapes.empty()) {
        return BoundingBox();
    }

    glm::vec3 min(std::numeric_limits<float>::max());
    glm::vec3 max(std::numeric_limits<float>::lowest());
    bool hasVertex = false;

    for (const auto &shape : mesh.shapes) {
        for (const auto &index : shape.mesh.indices) {
            const int vertexIndex = index.vertex_index;
            if (vertexIndex < 0) {
                continue;
            }

            const size_t base = static_cast<size_t>(vertexIndex) * 3;
            if (base + 2 >= mesh.attrib.vertices.size()) {
                continue;
            }

            const glm::vec3 vertex(mesh.attrib.vertices[base], mesh.attrib.vertices[base + 1], mesh.attrib.vertices[base + 2]);
            min = glm::min(min, vertex);
            max = glm::max(max, vertex);
            hasVertex = true;
        }
    }

    if (!hasVertex) {
        return BoundingBox();
    }

    return BoundingBox(min, max);
}

BoundingSphere CollisionHelper::generateBoundingSphere(const Mesh &mesh)
{
    if (mesh.attrib.vertices.empty() || mesh.shapes.empty()) {
        return BoundingSphere();
    }

    glm::vec3 min(std::numeric_limits<float>::max());
    glm::vec3 max(std::numeric_limits<float>::lowest());
    bool hasVertex = false;

    for (const auto &shape : mesh.shapes) {
        for (const auto &index : shape.mesh.indices) {
            const int vertexIndex = index.vertex_index;
            if (vertexIndex < 0) {
                continue;
            }

            const size_t base = static_cast<size_t>(vertexIndex) * 3;
            if (base + 2 >= mesh.attrib.vertices.size()) {
                continue;
            }

            const glm::vec3 vertex(mesh.attrib.vertices[base], mesh.attrib.vertices[base + 1], mesh.attrib.vertices[base + 2]);
            min = glm::min(min, vertex);
            max = glm::max(max, vertex);
            hasVertex = true;
        }
    }

    if (!hasVertex) {
        return BoundingSphere();
    }

    const glm::vec3 center = (min + max) * 0.5f;
    float maxDistanceSquared = 0.0f;

    for (const auto &shape : mesh.shapes) {
        for (const auto &index : shape.mesh.indices) {
            const int vertexIndex = index.vertex_index;
            if (vertexIndex < 0) {
                continue;
            }

            const size_t base = static_cast<size_t>(vertexIndex) * 3;
            if (base + 2 >= mesh.attrib.vertices.size()) {
                continue;
            }

            const glm::vec3 vertex(mesh.attrib.vertices[base], mesh.attrib.vertices[base + 1], mesh.attrib.vertices[base + 2]);
            const glm::vec3 delta = vertex - center;
            const float distanceSquared = glm::dot(delta, delta);
            if (distanceSquared > maxDistanceSquared) {
                maxDistanceSquared = distanceSquared;
            }
        }
    }

    return BoundingSphere(center, std::sqrt(maxDistanceSquared));
}
