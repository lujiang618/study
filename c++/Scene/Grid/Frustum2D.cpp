#include "Frustum2D.h"

Frustum2D::Frustum2D(const Vec2& camPos, const Vec2& direction, float nearDist, float farDist, float fovDegrees)
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
    nearLeft        = nearCenter - right * nearHalfWidth;
    nearRight       = nearCenter + right * nearHalfWidth;

    // 计算远裁剪平面的端点
    Vec2 farCenter = camPos + direction * farDist;
    farLeft        = farCenter - right * farHalfWidth;
    farRight       = farCenter + right * farHalfWidth;
}

void Frustum2D::Print()
{
    std::cout << "Frustum2D: " << std::endl;
    std::cout << "nearLeft: " << nearLeft << std::endl;
    std::cout << "nearRight: " << nearRight << std::endl;
    std::cout << "farLeft: " << farLeft << std::endl;
    std::cout << "farRight: " << farRight << std::endl;
}

Vec2 Frustum2D::GetNormal(const Vec2& edge) const
{
    return Vec2(-edge.y, edge.x).normalize();
}

bool Frustum2D::Overlaps(float min1, float max1, float min2, float max2) const
{
    return max1 >= min2 && max2 >= min1;
}

bool Frustum2D::Intersect1(const AABB& aabb) const
{
    // 提取视锥的边
    std::vector<Vec2> frustumEdges = {
        nearRight - nearLeft,
        farLeft - nearLeft,
        farRight - nearRight,
        farRight - farLeft};

    // 检查所有可能的分离轴
    for (const auto& edge : frustumEdges)
    {
        Vec2 normal = GetNormal(edge);

        // 计算视锥在法向量上的投影
        float frustumMin = std::min({nearLeft.dot(normal), nearRight.dot(normal),
                                     farLeft.dot(normal), farRight.dot(normal)});
        float frustumMax = std::max({nearLeft.dot(normal), nearRight.dot(normal),
                                     farLeft.dot(normal), farRight.dot(normal)});

        // 计算 AABB 在法向量上的投影
        float aabbMin = std::min({aabb.min.dot(normal), aabb.max.dot(normal)});
        float aabbMax = std::max({aabb.min.dot(normal), aabb.max.dot(normal)});

        // 如果投影不重叠，则不相交
        if (!Overlaps(frustumMin, frustumMax, aabbMin, aabbMax))
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
        Vec2 normal = GetNormal(edge);

        // 计算视锥在法向量上的投影
        float frustumMin = std::min({nearLeft.dot(normal), nearRight.dot(normal),
                                     farLeft.dot(normal), farRight.dot(normal)});
        float frustumMax = std::max({nearLeft.dot(normal), nearRight.dot(normal),
                                     farLeft.dot(normal), farRight.dot(normal)});

        // 计算 AABB 在法向量上的投影
        float aabbMin = std::min({aabb.min.dot(normal), aabb.max.dot(normal)});
        float aabbMax = std::max({aabb.min.dot(normal), aabb.max.dot(normal)});

        // 如果投影不重叠，则不相交
        if (!Overlaps(frustumMin, frustumMax, aabbMin, aabbMax))
        {
            return false;
        }
    }

    // 所有轴都无法分离，说明相交
    return true;
}

bool Frustum2D::isPointInFrustum(const Vec2& point) const
{
    // 将视锥的四个端点按顺时针顺序排列
    std::vector<Vec2> vertices = {nearLeft, nearRight, farRight, farLeft};

    // 初始化叉积符号f
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
bool Frustum2D::Intersect(const AABB& aabb) const
{
    auto corners = aabb.GetCorners();

    for (const auto& corner : corners)
    {
        if (isPointInFrustum(corner))
        {
            return true;
        }
    }

    return false;
}