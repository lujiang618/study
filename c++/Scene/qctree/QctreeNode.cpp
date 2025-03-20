#include "QctreeNode.h"

#include <bitset>
#include <iostream>

OctreeNode::OctreeNode(const AABB& bounds, int depth, int maxObjects, const std::string& code)
    : bounds(bounds)
    , depth(depth)
    , maxObjects(maxObjects)
    , code(code)
{
}

// 插入对象
void OctreeNode::insert(const Object& object, int maxDepth)
{
    if (isLeafNode())
    {
        // 如果是叶子节点，直接存储对象
        objects.push_back(object);
        if (objects.size() > maxObjects && depth < maxDepth)
        {
            // 如果对象数量超过限制，且未达到最大深度，生成子节点
            subdivide();
            redistributeObjects(maxDepth);
        }
    }
    else
    {
        // 如果不是叶子节点，递归插入到合适的子节点
        int childIndex = getChildIndex(object.position);
        std::cout << "****************************************************************" << std::endl;
        std::cout << "depth: " << depth << " index" << childIndex << std::endl;
        children[childIndex]->insert(object, maxDepth);
    }
}

void OctreeNode::query(const AABB& range, std::vector<Object>& result)
{
    // 如果当前节点的边界与查询区域不相交，直接返回
    if (!bounds.intersects(range))
    {
        return;
    }

    if (isLeafNode())
    {
        // 如果是叶子节点，将符合条件的对象加入结果
        for (const Object& object : objects)
        {
            if (range.contains(object.position))
            {
                result.push_back(object);
            }
        }
    }
    else
    {
        // 递归查询子节点
        for (int i = 0; i < 8; ++i)
        {
            if (children[i])
            {
                children[i]->query(range, result);
            }
        }
    }
}

std::string OctreeNode::getCode() const
{
    return code;
}

std::vector<Object> OctreeNode::getObjects() const
{
    return objects;
}

// 判断是否为叶子节点
bool OctreeNode::isLeafNode() const
{
    return children[0] == nullptr; // 如果没有子节点，则为叶子节点
}

// 判断是否有子节点
bool OctreeNode::hasChildren() const
{
    for (int i = 0; i < 8; ++i)
    {
        if (children[i])
        {

            return true;
        }
    }
    return false;
}

// 生成子节点
void OctreeNode::subdivide()
{
    Vector3 mid = {
        (bounds.min.x + bounds.max.x) / 2,
        (bounds.min.y + bounds.max.y) / 2,
        (bounds.min.z + bounds.max.z) / 2};
    for (int i = 0; i < 8; ++i)
    {
        AABB        childBounds = calculateChildBounds(i, mid);
        std::string childCode   = code + std::bitset<3>(i).to_string();
        children[i]             = std::make_unique<OctreeNode>(childBounds, depth + 1, maxObjects, childCode);
    }
}

// 将当前节点的对象重新分配到子节点
void OctreeNode::redistributeObjects(int maxDepth)
{
    for (auto it = objects.begin(); it != objects.end();)
    {
        int childIndex = getChildIndex(it->position);
        std::cout << "****************************************************************" << std::endl;
        std::cout << it->id << " depth: " << depth << " index " << childIndex << std::endl;
        std::cout << code << std::endl;
        children[childIndex]->insert(*it, maxDepth);
        it = objects.erase(it);
    }
}

// 计算子节点的边界
AABB OctreeNode::calculateChildBounds(int childIndex, const Vector3& mid)
{
    AABB childBounds = bounds;
    if (childIndex & (1 << 0))
        childBounds.min.x = mid.x;
    else
        childBounds.max.x = mid.x;
    if (childIndex & (1 << 1))
        childBounds.min.y = mid.y;
    else
        childBounds.max.y = mid.y;
    if (childIndex & (1 << 2))
        childBounds.min.z = mid.z;
    else
        childBounds.max.z = mid.z;
    return childBounds;
}

// 获取对象所属的子节点索引
int OctreeNode::getChildIndex(const Vector3& point) const
{
    int     childIndex = 0;
    Vector3 mid        = {
        (bounds.min.x + bounds.max.x) / 2,
        (bounds.min.y + bounds.max.y) / 2,
        (bounds.min.z + bounds.max.z) / 2};
    if (point.x >= mid.x)
        childIndex |= 1 << 0;
    if (point.y >= mid.y)
        childIndex |= 1 << 1;
    if (point.z >= mid.z)
        childIndex |= 1 << 2;
    return childIndex;
}
