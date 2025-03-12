#include "Tree.h"

std::unordered_map<int, AABB> globalObjectBounds;

Octree::Octree(const AABB& bounds, int maxDepth, int maxObjectsPerNode, std::unordered_map<int, AABB>& objectBounds)
    : root(std::make_unique<OctreeNode>(bounds))
    , maxDepth(maxDepth)
    , maxObjectsPerNode(maxObjectsPerNode)
    , globalObjectBounds(objectBounds)
{
}

// 插入对象
void Octree::insert(int objectId)
{
    const AABB& objectBounds = globalObjectBounds[objectId];
    insertRecursive(root.get(), objectId, objectBounds, 0);
}

// 查询与范围相交的对象
std::vector<int> Octree::query(const AABB& range) const
{
    std::unordered_set<int> resultSet; // 使用 unordered_set 去重
    queryRecursive(root.get(), range, resultSet);
    return std::vector<int>(resultSet.begin(), resultSet.end());
}

// 检测与目标对象的碰撞
std::vector<int> Octree::detectCollisions(int objectId) const
{
    const AABB&      objectBounds = globalObjectBounds.at(objectId);
    auto             candidates   = query(objectBounds);
    std::vector<int> collisions;
    for (int candidateId : candidates)
    {
        if (candidateId != objectId)
        {
            collisions.push_back(candidateId);
        }
    }
    return collisions;
}

// 递归插入对象
void Octree::insertRecursive(OctreeNode* node, int objectId, const AABB& objectBounds, int depth)
{
    if (node->isLeaf)
    {
        node->objects.push_back(objectId);
        if (node->objects.size() > maxObjectsPerNode && depth < maxDepth)
        {
            splitNode(node); // 分裂节点
        }
    }
    else
    {
        for (auto& child : node->children)
        {
            if (child->bounds.intersects(objectBounds))
            {
                insertRecursive(child.get(), objectId, objectBounds, depth + 1);
            }
        }
    }
}

// 分裂节点
void Octree::splitNode(OctreeNode* node)
{
    const auto& parentBounds = node->bounds;
    const float midX         = (parentBounds.minX + parentBounds.maxX) / 2;
    const float midY         = (parentBounds.minY + parentBounds.maxY) / 2;
    const float midZ         = (parentBounds.minZ + parentBounds.maxZ) / 2;

    node->children[0] = std::make_unique<OctreeNode>(AABB{parentBounds.minX, parentBounds.minY, parentBounds.minZ, midX, midY, midZ});
    node->children[1] = std::make_unique<OctreeNode>(AABB{midX, parentBounds.minY, parentBounds.minZ, parentBounds.maxX, midY, midZ});
    node->children[2] = std::make_unique<OctreeNode>(AABB{parentBounds.minX, midY, parentBounds.minZ, midX, parentBounds.maxY, midZ});
    node->children[3] = std::make_unique<OctreeNode>(AABB{midX, midY, parentBounds.minZ, parentBounds.maxX, parentBounds.maxY, midZ});
    node->children[4] = std::make_unique<OctreeNode>(AABB{parentBounds.minX, parentBounds.minY, midZ, midX, midY, parentBounds.maxZ});
    node->children[5] = std::make_unique<OctreeNode>(AABB{midX, parentBounds.minY, midZ, parentBounds.maxX, midY, parentBounds.maxZ});
    node->children[6] = std::make_unique<OctreeNode>(AABB{parentBounds.minX, midY, midZ, midX, parentBounds.maxY, parentBounds.maxZ});
    node->children[7] = std::make_unique<OctreeNode>(AABB{midX, midY, midZ, parentBounds.maxX, parentBounds.maxY, parentBounds.maxZ});

    for (int objectId : node->objects)
    {
        const AABB& objectBounds = globalObjectBounds[objectId];
        for (auto& child : node->children)
        {
            if (child->bounds.intersects(objectBounds))
            {
                child->objects.push_back(objectId);
            }
        }
    }

    node->objects.clear();
    node->isLeaf = false;
}

// 递归查询与范围相交的对象
void Octree::queryRecursive(const OctreeNode* node, const AABB& range, std::unordered_set<int>& resultSet) const
{
    if (!node->bounds.intersects(range))
    {
        return;
    }

    for (int objectId : node->objects)
    {
        const AABB& objectBounds = globalObjectBounds.at(objectId);
        if (range.intersects(objectBounds))
        {
            resultSet.insert(objectId); // 插入到 set 中
        }
    }

    if (!node->isLeaf)
    {
        for (const auto& child : node->children)
        {
            queryRecursive(child.get(), range, resultSet);
        }
    }
}