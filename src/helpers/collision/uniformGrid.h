#ifndef UNIFORM_GRID_H
#define UNIFORM_GRID_H

#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <glm/vec3.hpp>

#include "colliders/boundingBox.h"
#include "colliders/boundingSphere.h"

enum class ColliderShape
{
    Sphere,
    Box
};

struct CollisionProxy
{
    ColliderShape shape;
    const BoundingSphere *sphere = nullptr;
    const BoundingBox *box = nullptr;
    glm::vec3 position{0.0f};
    glm::vec3 scale{1.0f};
    glm::vec3 broadCenter{0.0f};
    float broadRadius = 0.0f;
};

class UniformGridBroadPhase
{
public:
    explicit UniformGridBroadPhase(float cellSize);

    void clear();
    void addBoundingSphere(const BoundingSphere &sphere, const glm::vec3 &position, const glm::vec3 &scale);
    void addBoundingBox(const BoundingBox &box, const glm::vec3 &position, const glm::vec3 &scale);

    template <typename Fn> void forEachPotentialPair(Fn &&callback) const
    {
        std::unordered_set<std::uint64_t> visitedPairs;
        visitedPairs.reserve(proxies.size() * 2);

        for (const auto &bucket : buckets) {
            const std::vector<std::size_t> &indices = bucket.second;
            if (indices.size() < 2) {
                continue;
            }

            for (std::size_t i = 0; i + 1 < indices.size(); ++i) {
                for (std::size_t j = i + 1; j < indices.size(); ++j) {
                    const std::size_t a = indices[i];
                    const std::size_t b = indices[j];
                    const std::uint64_t key = makePairKey(a, b);
                    if (visitedPairs.insert(key).second) {
                        callback(proxies[a], proxies[b]);
                    }
                }
            }
        }
    }

private:
    struct CellKey
    {
        int x = 0;
        int y = 0;
        int z = 0;

        bool operator==(const CellKey &other) const { return x == other.x && y == other.y && z == other.z; }
    };

    struct CellKeyHasher
    {
        std::size_t operator()(const CellKey &key) const
        {
            std::size_t h1 = std::hash<int>{}(key.x);
            std::size_t h2 = std::hash<int>{}(key.y);
            std::size_t h3 = std::hash<int>{}(key.z);
            return (h1 ^ (h2 << 1)) ^ (h3 << 2);
        }
    };

    float cellSize;
    float invCellSize;
    std::unordered_map<CellKey, std::vector<std::size_t>, CellKeyHasher> buckets;
    std::vector<CollisionProxy> proxies;

    static std::uint64_t makePairKey(std::size_t a, std::size_t b)
    {
        const std::size_t minIndex = a < b ? a : b;
        const std::size_t maxIndex = a < b ? b : a;
        return (static_cast<std::uint64_t>(minIndex) << 32) | static_cast<std::uint64_t>(maxIndex);
    }

    void insertProxy(const CollisionProxy &proxy);
    void addProxyToCells(std::size_t proxyIndex, const glm::vec3 &center, float radius);
};

#endif
