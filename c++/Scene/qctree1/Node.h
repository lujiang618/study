

#include <vector>
#include <array>
#include <memory>
#include <algorithm>

struct AABB {
    float minX, minY, minZ;
    float maxX, maxY, maxZ;

    bool contains(float x, float y, float z) const {
        return x >= minX && x <= maxX &&
               y >= minY && y <= maxY &&
               z >= minZ && z <= maxZ;
    }

    bool intersects(const AABB& other) const {
        return !(maxX < other.minX || minX > other.maxX ||
                 maxY < other.minY || minY > other.maxY ||
                 maxZ < other.minZ || minZ > other.maxZ);
    }
};

struct OctreeNode {
    AABB bounds; // 节点表示的空间范围
    std::vector<int> objects; // 存储在该节点中的对象 ID
    std::array<std::unique_ptr<OctreeNode>, 8> children; // 8 个子节点
    bool isLeaf; // 是否为叶子节点

    OctreeNode(const AABB& bounds) : bounds(bounds), isLeaf(true) {}
};