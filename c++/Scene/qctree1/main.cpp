#include <iostream>

#include "Tree.h"

int main()
{
    // 定义场景的边界
    AABB sceneBounds{0, 0, 0, 100, 100, 100};

    // 全局容器存储对象的包围盒
    std::unordered_map<int, AABB> globalObjectBounds;

    // 创建八叉树
    Octree octree(sceneBounds, 5, 10, globalObjectBounds);

    // 插入对象
    for (int i = 0; i < 20; ++i)
    {
        AABB objectBounds{float(i * 5), float(i * 5), float(i * 5), float(i * 5 + 5), float(i * 5 + 5), float(i * 5 + 5)};
        globalObjectBounds[i] = objectBounds; // 存储包围盒
        octree.insert(i);                     // 插入对象
    }

    // 打印所有对象的包围盒
    std::cout << "Object bounds:\n";
    for (const auto& [id, bounds] : globalObjectBounds)
    {
        std::cout << "Object " << id << ": (" << bounds.minX << ", " << bounds.minY << ", " << bounds.minZ << ") - ("
                  << bounds.maxX << ", " << bounds.maxY << ", " << bounds.maxZ << ")\n";
    }

    // 查询与范围相交的对象
    AABB queryRange{10, 10, 10, 50, 50, 50};
    auto result = octree.query(queryRange);
    std::cout << "Objects in range: ";
    for (int id : result)
    {
        std::cout << id << " ";
    }
    std::cout << std::endl;

    // 检测碰撞
    int  targetObjectId = 5;
    auto collisions     = octree.detectCollisions(targetObjectId);
    std::cout << "Collisions with object " << targetObjectId << ": ";
    for (int id : collisions)
    {
        std::cout << id << " ";
    }
    std::cout << std::endl;

    return 0;
}