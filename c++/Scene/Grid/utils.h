#include <cmath>
#include <iostream>
#include <vector>

struct Vec2
{
    float x, y;

    Vec2(float x = 0, float y = 0)
        : x(x)
        , y(y)
    {
    }

    bool operator==(const Vec2& other) const
    {
        return x == other.x && y == other.y;
    }

    // 向量加法
    Vec2 operator+(const Vec2& other) const
    {
        return Vec2(x + other.x, y + other.y);
    }

    // 向量减法
    Vec2 operator-(const Vec2& other) const
    {
        return Vec2(x - other.x, y - other.y);
    }

    // 乘以标量（数乘）
    Vec2 operator*(float scalar) const
    {
        return Vec2(x * scalar, y * scalar);
    }

    // 除以标量
    Vec2 operator/(float scalar) const
    {
        return Vec2(x / scalar, y / scalar);
    }

    // 计算向量长度
    float Length() const
    {
        return std::sqrt(x * x + y * y);
    }

    // 归一化向量
    Vec2 normalize() const
    {
        float len = Length();
        return Vec2(x / len, y / len);
    }

    // 计算点积
    float dot(const Vec2& other) const
    {
        return x * other.x + y * other.y;
    }

    // 计算叉积（返回标量）
    float cross(const Vec2& other) const
    {
        return x * other.y - y * other.x;
    }

    // 输出运算符重载
    friend std::ostream& operator<<(std::ostream& os, const Vec2& v)
    {
        os << "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
        return os;
    }
};

// 2D 视锥 (AABB) 矩形（Bounding Box）
struct AABB
{
    Vec2 min; // 最小点
    Vec2 max; // 最大点

    std::vector<Vec2> GetCorners() const
    {
        return {min, {max.x, min.y}, max, {min.x, max.y}};
    }
};

// 计算 Vec2 哈希值，使其可用作 unordered_map 的 key
namespace std
{
template <>
struct hash<Vec2>
{
    size_t operator()(const Vec2& v) const
    {
        return hash<float>()(v.x) ^ (hash<float>()(v.y) << 1);
    }
};
} // namespace std

struct Vec3
{
    float x, y, z;

    Vec3(float x = 0, float y = 0, float z = 0)
        : x(x)
        , y(y)
        , z(z)
    {
    }

    bool operator==(const Vec3& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    float dot(const Vec3& other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }
};

struct Plane
{
    Vec3  normal; // 法向量
    float d;      // 偏移量

    // 计算点到平面的距离
    float Distance(const Vec3& point) const
    {
        return normal.dot(point) + d;
    }
};

struct Frustum
{
    float minX, maxX, minY, maxY;

    Frustum(float minX, float maxX, float minY, float maxY)
        : minX(minX)
        , maxX(maxX)
        , minY(minY)
        , maxY(maxY)
    {
    }

    bool Contains(float x, float y) const
    {
        return x >= minX && x <= maxX && y >= minY && y <= maxY;
    }
};