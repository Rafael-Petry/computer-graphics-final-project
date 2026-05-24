#include <algorithm>
#include <cmath>

#include <glm/common.hpp>
#include <glm/geometric.hpp>

#include "uniformGrid.h"

namespace {
    float maxAbsComponent(const glm::vec3 &value) { return std::max(std::max(std::fabs(value.x), std::fabs(value.y)), std::fabs(value.z)); }
}

UniformGridBroadPhase::UniformGridBroadPhase(float cellSize) : cellSize(cellSize), invCellSize(cellSize > 0.0f ? 1.0f / cellSize : 0.0f) {}

void UniformGridBroadPhase::clear()
{
    buckets.clear();
    proxies.clear();
}

void UniformGridBroadPhase::addBoundingSphere(const BoundingSphere &sphere, const glm::vec3 &position, const glm::vec3 &scale)
{
    if (!sphere.isInitialized()) {
        return;
    }

    CollisionProxy proxy;
    proxy.shape = ColliderShape::Sphere;
    proxy.sphere = &sphere;
    proxy.position = position;
    proxy.scale = scale;
    proxy.broadCenter = (sphere.getCenter() * scale) + position;
    proxy.broadRadius = sphere.getRadius() * maxAbsComponent(scale);

    insertProxy(proxy);
}

void UniformGridBroadPhase::addBoundingBox(const BoundingBox &box, const glm::vec3 &position, const glm::vec3 &scale)
{
    if (!box.isInitialized()) {
        return;
    }

    const glm::vec3 localCenter = (box.getMin() + box.getMax()) * 0.5f;
    const glm::vec3 halfExtents = (box.getMax() - box.getMin()) * 0.5f;
    const glm::vec3 absScale = glm::abs(scale);
    const glm::vec3 worldExtents = halfExtents * absScale;

    CollisionProxy proxy;
    proxy.shape = ColliderShape::Box;
    proxy.box = &box;
    proxy.position = position;
    proxy.scale = scale;
    proxy.broadCenter = (localCenter * scale) + position;
    proxy.broadRadius = std::sqrt(glm::dot(worldExtents, worldExtents));

    insertProxy(proxy);
}

void UniformGridBroadPhase::insertProxy(const CollisionProxy &proxy)
{
    const std::size_t index = proxies.size();
    proxies.push_back(proxy);

    addProxyToCells(index, proxy.broadCenter, proxy.broadRadius);
}

void UniformGridBroadPhase::addProxyToCells(std::size_t proxyIndex, const glm::vec3 &center, float radius)
{
    if (cellSize <= 0.0f) {
        return;
    }

    const glm::vec3 minPoint = center - glm::vec3(radius);
    const glm::vec3 maxPoint = center + glm::vec3(radius);

    const int minX = static_cast<int>(std::floor(minPoint.x * invCellSize));
    const int minY = static_cast<int>(std::floor(minPoint.y * invCellSize));
    const int minZ = static_cast<int>(std::floor(minPoint.z * invCellSize));

    const int maxX = static_cast<int>(std::floor(maxPoint.x * invCellSize));
    const int maxY = static_cast<int>(std::floor(maxPoint.y * invCellSize));
    const int maxZ = static_cast<int>(std::floor(maxPoint.z * invCellSize));

    for (int x = minX; x <= maxX; ++x) {
        for (int y = minY; y <= maxY; ++y) {
            for (int z = minZ; z <= maxZ; ++z) {
                CellKey key;
                key.x = x;
                key.y = y;
                key.z = z;
                buckets[key].push_back(proxyIndex);
            }
        }
    }
}
