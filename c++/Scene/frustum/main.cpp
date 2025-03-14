#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

struct Vec2
{
    float x, y;

    Vec2(float x = 0, float y = 0)
        : x(x)
        , y(y)
    {
    }

    Vec2 operator+(const Vec2& other) const
    {
        return Vec2(x + other.x, y + other.y);
    }

    Vec2 operator-(const Vec2& other) const
    {
        return Vec2(x - other.x, y - other.y);
    }

    Vec2 operator*(float scalar) const
    {
        return Vec2(x * scalar, y * scalar);
    }

    Vec2 normalize() const
    {
        float length = std::sqrt(x * x + y * y);
        return Vec2(x / length, y / length);
    }

    float dot(const Vec2& other) const
    {
        return x * other.x + y * other.y;
    }

    float cross(const Vec2& other) const
    {
        return x * other.y - y * other.x;
    }
};

struct Frustum
{
    Vec2 nearLeft, nearRight; // 近裁剪平面的左右端点
    Vec2 farLeft, farRight;   // 远裁剪平面的左右端点
};

Frustum generateFrustum(const Vec2& camPos, const Vec2& direction, float nearDist, float farDist, float fovDegrees)
{
    // 将 FOV 从角度转换为弧度
    float fovRadians = fovDegrees * (M_PI / 180.0f);

    // 计算视野宽度的一半
    float nearHalfWidth = nearDist * std::tan(fovRadians / 2.0f);
    float farHalfWidth  = farDist * std::tan(fovRadians / 2.0f);

    // 计算右方向
    Vec2 right(direction.y, -direction.x); // 右方向是方向的垂直向量

    // 计算近裁剪平面的端点
    Vec2 nearCenter = camPos + direction * nearDist;
    Vec2 nearLeft   = nearCenter - right * nearHalfWidth;
    Vec2 nearRight  = nearCenter + right * nearHalfWidth;

    // 计算远裁剪平面的端点
    Vec2 farCenter = camPos + direction * farDist;
    Vec2 farLeft   = farCenter - right * farHalfWidth;
    Vec2 farRight  = farCenter + right * farHalfWidth;

    return {nearLeft, nearRight, farLeft, farRight};
}

// 计算边的法向量
Vec2 getNormal(const Vec2& edge)
{
    return Vec2(-edge.y, edge.x).normalize();
}

// 检查两个投影是否重叠
bool overlaps(float min1, float max1, float min2, float max2)
{
    return max1 >= min2 && max2 >= min1;
}

struct AABB
{
    Vec2 min; // 最小点
    Vec2 max; // 最大点

    std::vector<Vec2> GetCorners() const {
        return { min, {max.x, min.y}, max, {min.x, max.y} };
    }
};

// 检查视锥和 AABB 是否相交
// 分离轴
bool frustumAABBIntersect1(const Frustum& frustum, const AABB& aabb)
{
    // 提取视锥的边
    std::vector<Vec2> frustumEdges = {
        frustum.nearRight - frustum.nearLeft,
        frustum.farLeft - frustum.nearLeft,
        frustum.farRight - frustum.nearRight,
        frustum.farRight - frustum.farLeft};

    // 检查所有可能的分离轴
    for (const auto& edge : frustumEdges)
    {
        Vec2 normal = getNormal(edge);

        // 计算视锥在法向量上的投影
        float frustumMin = std::min({frustum.nearLeft.dot(normal), frustum.nearRight.dot(normal),
                                     frustum.farLeft.dot(normal), frustum.farRight.dot(normal)});
        float frustumMax = std::max({frustum.nearLeft.dot(normal), frustum.nearRight.dot(normal),
                                     frustum.farLeft.dot(normal), frustum.farRight.dot(normal)});

        // 计算 AABB 在法向量上的投影
        float aabbMin = std::min({aabb.min.dot(normal), aabb.max.dot(normal)});
        float aabbMax = std::max({aabb.min.dot(normal), aabb.max.dot(normal)});

        // 如果投影不重叠，则不相交
        if (!overlaps(frustumMin, frustumMax, aabbMin, aabbMax))
        {
            return false;
        }
    }

    // 提取 AABB 的边
    std::vector<Vec2> aabbEdges = {
        Vec2(1, 0), // X 轴
        Vec2(0, 1)  // Y 轴
    };

    for (const auto& edge : aabbEdges)
    {
        Vec2 normal = getNormal(edge);

        // 计算视锥在法向量上的投影
        float frustumMin = std::min({frustum.nearLeft.dot(normal), frustum.nearRight.dot(normal),
                                     frustum.farLeft.dot(normal), frustum.farRight.dot(normal)});
        float frustumMax = std::max({frustum.nearLeft.dot(normal), frustum.nearRight.dot(normal),
                                     frustum.farLeft.dot(normal), frustum.farRight.dot(normal)});

        // 计算 AABB 在法向量上的投影
        float aabbMin = std::min({aabb.min.dot(normal), aabb.max.dot(normal)});
        float aabbMax = std::max({aabb.min.dot(normal), aabb.max.dot(normal)});

        // 如果投影不重叠，则不相交
        if (!overlaps(frustumMin, frustumMax, aabbMin, aabbMax))
        {
            return false;
        }
    }

    // 所有轴都无法分离，说明相交
    return true;
}

// 判断点是否在视锥内
bool isPointInFrustum(const Frustum& frustum, const Vec2& point)
{
    // 将视锥的四个端点按顺时针顺序排列
    std::vector<Vec2> vertices = {frustum.nearLeft, frustum.nearRight, frustum.farRight, frustum.farLeft};

    // 初始化叉积符号
    int sign = 0;

    for (size_t i = 0; i < vertices.size(); i++)
    {
        size_t j       = (i + 1) % vertices.size(); // 下一个顶点
        Vec2   edge    = vertices[j] - vertices[i]; // 边向量
        Vec2   toPoint = point - vertices[i];       // 点到边的向量

        // 计算叉积
        float cross = edge.cross(toPoint);

        // 初始化符号
        if (sign == 0)
        {
            sign = (cross > 0) ? 1 : -1;
        }
        else
        {
            // 如果符号不一致，则点在视锥外
            if ((cross > 0 && sign == -1) || (cross < 0 && sign == 1))
            {
                return false;
            }
        }
    }

    // 所有叉积符号一致，点在视锥内
    return true;
}


// aabb的顶点是否在视锥内
bool frustumAABBIntersect(const Frustum& frustum, const AABB& aabb)
{
    // 1. 获取 AABB 的所有角点
    auto corners = aabb.GetCorners();

    for (const auto& corner : corners) {
        if (isPointInFrustum(frustum, corner)) {
            return true;
        }
    }

    return false;
}

int main()
{
    // 设置相机参数
    Vec2  camPos(30, 30);     // 相机位置
    Vec2  direction(0, 1);    // 相机方向（朝上）
    float nearDist   = 10.0f; // 近裁剪距离
    float farDist    = 40.0f; // 远裁剪距离
    float fovDegrees = 60.0f; // FOV（视野角度）

    // 生成视锥
    Frustum frustum = generateFrustum(camPos, direction, nearDist, farDist, fovDegrees);

    // 输出视锥的四个端点
    std::cout << "Near Left: (" << frustum.nearLeft.x << ", " << frustum.nearLeft.y << ")\n";
    std::cout << "Near Right: (" << frustum.nearRight.x << ", " << frustum.nearRight.y << ")\n";
    std::cout << "Far Left: (" << frustum.farLeft.x << ", " << frustum.farLeft.y << ")\n";
    std::cout << "Far Right: (" << frustum.farRight.x << ", " << frustum.farRight.y << ")\n";

    // 定义 AABB
    AABB aabb = {
        Vec2(25, 50), // min
        Vec2(35, 60)  // max
    };

    // 判断是否相交
    bool intersect = frustumAABBIntersect(frustum, aabb);
    std::cout << "Frustum and AABB " << (intersect ? "intersect" : "do not intersect") << ".\n";

    AABB aabb1 = {
        Vec2(5, 15), // min
        Vec2(20, 35) // max
    };

    // 判断是否相交
    intersect = frustumAABBIntersect(frustum, aabb1);
    std::cout << "Frustum and AABB " << (intersect ? "intersect" : "do not intersect") << ".\n";

    // 测试点
    Vec2 point(30, 50); // 测试点

    // 判断点是否在视锥内
    bool isInside = isPointInFrustum(frustum, point);
    std::cout << "Point (" << point.x << ", " << point.y << ") is "
              << (isInside ? "inside" : "outside") << " the frustum.\n";

    Vec2 point2(5, 5); // 测试点

    // 判断点是否在视锥内
    isInside = isPointInFrustum(frustum, point2);
    std::cout << "Point (" << point2.x << ", " << point2.y << ") is "
              << (isInside ? "inside" : "outside") << " the frustum.\n";

    return 0;
}