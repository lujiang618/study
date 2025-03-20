#pragma once

#include <unordered_map>
#include <vector>

#include "utils.h"
#include "QctreeNode.h"

class Qctree
{
public:
    Qctree(float size, int maxDepth, int maxObjects, const Vector3& origin = Vector3());

    void insert(const Object& object);

    std::vector<Object> query(const Vector3& point, float range);

    OctreeNode* locateLeafNode(const std::string& code);

    void printLeafNodes();

private:

OctreeNode* locateLeafNodeRecursive(OctreeNode* node, const std::string& code);

void printLeafNodesRecursive(OctreeNode* node);

//     std::string getNodeCode(const Vector3& point);

//     bool nodeIntersectsQuery(const std::string& code, const AABB& queryRange);

private:
    float size;

    int maxDepth;

    int maxObjects; // 节点最大对象数量

    std::unique_ptr<OctreeNode> root;
};