#include <iostream>
#include <vector>
#include <memory>
#include <bitset>
#include <unordered_map>

// 定义 Vector3 结构
struct Vector3 {
    float x, y, z;

    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
};

// 定义 Object 结构
struct Object {
    int id; // 对象的唯一标识符
    Vector3 position; // 对象的位置

    Object(int id, const Vector3& position) : id(id), position(position) {}
};

// 定义 AABB 结构
struct AABB {
    Vector3 min;
    Vector3 max;

    bool contains(const Vector3& point) const {
        return point.x >= min.x && point.x <= max.x &&
               point.y >= min.y && point.y <= max.y &&
               point.z >= min.z && point.z <= max.z;
    }

    bool intersects(const AABB& other) const {
        return !(max.x < other.min.x || min.x > other.max.x ||
                 max.y < other.min.y || min.y > other.max.y ||
                 max.z < other.min.z || min.z > other.max.z);
    }
};

// 定义 OctreeNode 类
class OctreeNode {
public:
    AABB bounds; // 节点的边界
    std::vector<Object> objects; // 节点中的对象（仅叶子节点存储对象）
    std::unique_ptr<OctreeNode> children[8]; // 子节点
    int depth; // 当前节点的深度
    int maxObjects; // 节点最大存储数量
    std::string code; // 节点的二进制编码

    OctreeNode(const AABB& bounds, int depth, int maxObjects, const std::string& code = "")
        : bounds(bounds), depth(depth), maxObjects(maxObjects), code(code) {}

    // 插入对象
    void insert(const Object& object, int maxDepth) {
        if (isLeafNode()) {
            // 如果是叶子节点，直接存储对象
            objects.push_back(object);
            if (objects.size() > maxObjects && depth < maxDepth) {
                // 如果对象数量超过限制，且未达到最大深度，生成子节点
                subdivide();
                redistributeObjects(maxDepth);
            }
        } else {
            // 如果不是叶子节点，递归插入到合适的子节点
            int childIndex = getChildIndex(object.position);
            children[childIndex]->insert(object, maxDepth);
        }
    }

    // 查询对象
    void query(const AABB& range, std::vector<Object>& result) {
        // 如果当前节点的边界与查询区域不相交，直接返回
        if (!bounds.intersects(range)) {
            return;
        }

        if (isLeafNode()) {
            // 如果是叶子节点，将符合条件的对象加入结果
            for (const Object& object : objects) {
                if (range.contains(object.position)) {
                    result.push_back(object);
                }
            }
        } else {
            // 递归查询子节点
            for (int i = 0; i < 8; ++i) {
                if (children[i]) {
                    children[i]->query(range, result);
                }
            }
        }
    }

    // 获取叶子节点中的对象
    std::vector<Object> getObjects() const {
        return objects;
    }

    // 获取节点的二进制编码
    std::string getCode() const {
        return code;
    }

    // 判断是否为叶子节点
    bool isLeafNode() const {
        return children[0] == nullptr; // 如果没有子节点，则为叶子节点
    }

private:
    // 生成子节点
    void subdivide() {
        Vector3 mid = {
            (bounds.min.x + bounds.max.x) / 2,
            (bounds.min.y + bounds.max.y) / 2,
            (bounds.min.z + bounds.max.z) / 2
        };
        for (int i = 0; i < 8; ++i) {
            AABB childBounds = calculateChildBounds(i, mid);
            std::string childCode = code + std::bitset<3>(i).to_string();
            children[i] = std::make_unique<OctreeNode>(childBounds, depth + 1, maxObjects, childCode);
        }
    }

    // 将当前节点的对象重新分配到子节点
    void redistributeObjects(int maxDepth) {
        for (auto it = objects.begin(); it != objects.end();) {
            int childIndex = getChildIndex(it->position);
            children[childIndex]->insert(*it, maxDepth);
            it = objects.erase(it);
        }
    }

    // 计算子节点的边界
    AABB calculateChildBounds(int childIndex, const Vector3& mid) {
        AABB childBounds = bounds;
        if (childIndex & (1 << 0)) childBounds.min.x = mid.x; else childBounds.max.x = mid.x;
        if (childIndex & (1 << 1)) childBounds.min.y = mid.y; else childBounds.max.y = mid.y;
        if (childIndex & (1 << 2)) childBounds.min.z = mid.z; else childBounds.max.z = mid.z;
        return childBounds;
    }

    // 获取对象所属的子节点索引
    int getChildIndex(const Vector3& point) const {
        int childIndex = 0;
        Vector3 mid = {
            (bounds.min.x + bounds.max.x) / 2,
            (bounds.min.y + bounds.max.y) / 2,
            (bounds.min.z + bounds.max.z) / 2
        };
        if (point.x >= mid.x) childIndex |= 1 << 0;
        if (point.y >= mid.y) childIndex |= 1 << 1;
        if (point.z >= mid.z) childIndex |= 1 << 2;
        return childIndex;
    }
};

// 定义 LinearOctree 类
class LinearOctree {
public:
    LinearOctree(float size, int maxDepth, int maxObjects, const Vector3& origin = Vector3())
        : size(size), maxDepth(maxDepth), maxObjects(maxObjects) {
        AABB rootBounds = {origin, {origin.x + size, origin.y + size, origin.z + size}};
        root = std::make_unique<OctreeNode>(rootBounds, 0, maxObjects, "0");
    }

    // 插入对象
    void insert(const Object& object) {
        root->insert(object, maxDepth);
    }

    // 查询对象
    std::vector<Object> query(const Vector3& point, float range) {
        std::vector<Object> result;
        AABB queryRange = {
            {point.x - range, point.y - range, point.z - range},
            {point.x + range, point.y + range, point.z + range}
        };
        root->query(queryRange, result);
        return result;
    }

    // 通过二进制编码获取叶子节点
    OctreeNode* locateLeafNode(const std::string& code) {
        return locateLeafNodeRecursive(root.get(), code);
    }

    // 打印所有叶子节点的编码和对象
    void printLeafNodes() {
        printLeafNodesRecursive(root.get());
    }

private:
    float size; // 八叉树的总尺寸
    int maxDepth; // 八叉树的最大深度
    int maxObjects; // 节点最大存储数量
    std::unique_ptr<OctreeNode> root; // 根节点

    // 递归查找叶子节点
    OctreeNode* locateLeafNodeRecursive(OctreeNode* node, const std::string& code) {
        if (!node) return nullptr;
        if (node->getCode() == code) return node;
        for (int i = 0; i < 8; ++i) {
            OctreeNode* result = locateLeafNodeRecursive(node->children[i].get(), code);
            if (result) return result;
        }
        return nullptr;
    }

    // 递归打印所有叶子节点的编码和对象
    void printLeafNodesRecursive(OctreeNode* node) {
        if (!node) return;
        if (node->isLeafNode()) {
            std::cout << "Leaf Node Code: " << node->getCode() << std::endl;
            for (const Object& object : node->getObjects()) {
                std::cout << "  Object ID: " << object.id << ", Position: ("
                          << object.position.x << ", " << object.position.y << ", " << object.position.z << ")" << std::endl;
            }
        }
        for (int i = 0; i < 8; ++i) {
            printLeafNodesRecursive(node->children[i].get());
        }
    }
};

int main() {
    // 创建线性八叉树，尺寸为 1024，最大深度为 4，节点最大存储数量为 2，源点为 (0, 0, 0)
    LinearOctree octree(1024, 4, 2);

    // 插入对象
    octree.insert({1, {100, 200, 300}});
    octree.insert({2, {150, 250, 350}});
    octree.insert({3, {120, 220, 320}});

    // 查询对象
    auto result = octree.query({120, 220, 320}, 5);
    for (const Object& object : result) {
        std::cout << "Found object: ID = " << object.id << ", Position: ("
                  << object.position.x << ", " << object.position.y << ", " << object.position.z << ")" << std::endl;
    }

    // 打印所有叶子节点的编码和对象
    octree.printLeafNodes();

    // 通过二进制编码获取叶子节点
    OctreeNode* leafNode = octree.locateLeafNode("0000");
    if (leafNode) {
        std::cout << "Located leaf node: " << leafNode->getCode() << std::endl;
        for (const Object& object : leafNode->getObjects()) {
            std::cout << "  Object ID: " << object.id << ", Position: ("
                      << object.position.x << ", " << object.position.y << ", " << object.position.z << ")" << std::endl;
        }
    }

    return 0;
}