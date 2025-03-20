#pragma once

#include <memory>
#include <vector>

#include "utils.h"

class OctreeNode
{
public:
    AABB                        bounds;      // 节点的边界
    std::vector<Object>         objects;     // 节点中的对象
    std::unique_ptr<OctreeNode> children[8]; // 子节点
    int                         depth;       // 当前节点的深度
    int                         maxObjects;  // 节点最大存储数量
    std::string                 code;        // 节点的二进制编码

    OctreeNode(const AABB& bounds, int depth, int maxObjects, const std::string& code = "");

    // 插入对象
    void insert(const Object& object, int maxDepth);

    void query(const AABB& range, std::vector<Object>& result);

    std::vector<Object> getObjects() const;

    std::string getCode() const;

    bool isLeafNode() const;

private:
    bool hasChildren() const;

    void subdivide();

    void redistributeObjects(int maxDepth);

    AABB calculateChildBounds(int childIndex, const Vector3& mid);

    int getChildIndex(const Vector3& point) const;

    int getChildIndexForObject(int objectId) const;

    // 计算子节点的边界
};