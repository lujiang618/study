
#pragma once

#include "Node.h"
#include <unordered_map>
#include <unordered_set>



class Octree
{
public:
    Octree(const AABB& bounds, int maxDepth, int maxObjectsPerNode, std::unordered_map<int, AABB>& objectBounds);

    // 插入对象
    void insert(int objectId);
    // 查询与范围相交的对象
    std::vector<int> query(const AABB& range) const;

    // 检测与目标对象的碰撞
    std::vector<int> detectCollisions(int objectId) const;

private:
    // 递归插入对象
    void insertRecursive(OctreeNode* node, int objectId, const AABB& objectBounds, int depth);

    // 分裂节点
    void splitNode(OctreeNode* node);

    // 递归查询与范围相交的对象
    void queryRecursive(const OctreeNode* node, const AABB& range, std::unordered_set<int>& resultSet) const;

private:
    std::unique_ptr<OctreeNode>    root;
    int                            maxDepth;           // 最大深度
    int                            maxObjectsPerNode;  // 每个节点中对象的最大数量
    std::unordered_map<int, AABB>& globalObjectBounds; // 引用全局容器
};