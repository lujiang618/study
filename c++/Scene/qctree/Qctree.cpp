#include "Qctree.h"

#include <bitset>
#include <iostream>

Qctree::Qctree(float size, int depth, int maxObjects, const Vector3& origin)
    : size(size)
    , maxDepth(depth)
    , maxObjects(maxObjects)
{
    AABB rootBounds = {origin, {origin.x + size, origin.y + size, origin.z + size}};

    root = std::make_unique<OctreeNode>(rootBounds, 0, maxObjects);
}

void Qctree::insert(const Object& object)
{
    root->insert(object, maxDepth);
}

std::vector<Object> Qctree::query(const Vector3& point, float range)
{
    std::vector<Object> result;

    AABB queryRange = {
        {point.x - range, point.y - range, point.z - range},
        {point.x + range, point.y + range, point.z + range}};

    root->query(queryRange, result);

    return result;
}

// 通过二进制编码获取叶子节点
OctreeNode* Qctree::locateLeafNode(const std::string& code)
{
    return locateLeafNodeRecursive(root.get(), code);
}

// 打印所有叶子节点的编码和对象
void Qctree::printLeafNodes()
{
    printLeafNodesRecursive(root.get());
}

// 递归查找叶子节点
OctreeNode* Qctree::locateLeafNodeRecursive(OctreeNode* node, const std::string& code)
{
    if (!node)
        return nullptr;
    if (node->getCode() == code)
        return node;
    for (int i = 0; i < 8; ++i)
    {
        OctreeNode* result = locateLeafNodeRecursive(node->children[i].get(), code);
        if (result)
            return result;
    }
    return nullptr;
}

// 递归打印所有叶子节点的编码和对象
void Qctree::printLeafNodesRecursive(OctreeNode* node)
{
    if (!node)
        return;
    if (node->isLeafNode())
    {
        std::cout << "Leaf Node Code: " << node->getCode() << std::endl;
        for (const Object& object : node->getObjects())
        {
            std::cout << "  Object ID: " << object.id << ", Position: ("
                      << object.position.x << ", " << object.position.y << ", " << object.position.z << ")" << std::endl;
        }
    }

    for (int i = 0; i < 8; ++i)
    {
        printLeafNodesRecursive(node->children[i].get());
    }
}