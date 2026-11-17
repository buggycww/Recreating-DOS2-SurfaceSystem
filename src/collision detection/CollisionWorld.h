#pragma once

#include "AABB.h"
#include "Octree.h"

#include <cstdint>
#include <memory>
#include <raylib.h>
#include <vector>

class GameObject;

struct RayHit {
    GameObject* object = nullptr;
    Vector3 point{};
    Vector3 normal{};
    float distance = 0.f;
};

// Spatial index for the level's solid colliders.
//
// Static geometry never moves, so the tree is built once after the map is
// loaded. The caller passes the objects that should take part in collision;
// the world-space bounds are derived from each object's Transform3D.
class CollisionWorld {
public:
    void build(const std::vector<GameObject*>& objects);
    void clear();
    void query(const AABB& area, std::vector<GameObject*>& out) const;
    bool overlaps(const AABB& area) const;

    // Nearest collider hit along the ray, using the octree as the broad phase.
    bool raycast(const Ray& ray, float maxDistance, RayHit& outHit) const;

    bool empty() const { return tree == nullptr; }
    int nodeCount() const { return tree ? tree->countNodes() : 0; }
    int entryCount() const { return tree ? tree->countEntries() : 0; }

private:
    std::unique_ptr<Octree> tree;

    // The octree stores an index into owners, keeping the tree independent of
    // game types.
    std::vector<GameObject*> owners;
};
