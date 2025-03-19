#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>

#include "utils.h"

class Frustum2D
{

public:
    Frustum2D(const Vec2& camPos, const Vec2& direction, float nearDist, float farDist, float fovDegrees);

    void Print();

    // 计算边的法向量
    Vec2 GetNormal(const Vec2& edge) const;

    // 检查两个投影是否重叠
    bool Overlaps(float min1, float max1, float min2, float max2) const;

    bool Intersect1(const AABB& aabb) const;

    bool isPointInFrustum(const Vec2& point) const;

    // aabb的顶点是否在视锥内
    bool Intersect(const AABB& aabb) const;

private:
    Vec2 nearLeft;  // 近平面左上角，右上角

    Vec2 nearRight; // 近平面左上角，右上角

    Vec2 farLeft;   // 远平面左下角，右下角
    
    Vec2 farRight;  // 远平面左下角，右下角
};