#include <iostream>

#include "Qctree.h"

int main()
{
    // 创建线性八叉树，尺寸为 1024，最大深度为 4，节点最大存储数量为 2，源点为 (0, 0, 0)
    Qctree octree(1024, 4, 2);

    // 插入对象
    octree.insert({1, {100, 200, 300}});
    octree.insert({2, {150, 250, 350}});
    octree.insert({3, {120, 220, 320}});

    // 查询对象
    auto result = octree.query({120, 220, 320}, 50);
    for (const Object& object : result)
    {
        std::cout << "Found object: ID = " << object.id << ", Position: ("
                  << object.position.x << ", " << object.position.y << ", " << object.position.z << ")" << std::endl;
    }

    // 打印所有叶子节点的编码和对象
    octree.printLeafNodes();

    // 通过二进制编码获取叶子节点
    OctreeNode* leafNode = octree.locateLeafNode("0000");
    if (leafNode)
    {
        std::cout << "Located leaf node: " << leafNode->getCode() << std::endl;
        for (const Object& object : leafNode->getObjects())
        {
            std::cout << "  Object ID: " << object.id << ", Position: ("
                      << object.position.x << ", " << object.position.y << ", " << object.position.z << ")" << std::endl;
        }
    }

    return 0;
}