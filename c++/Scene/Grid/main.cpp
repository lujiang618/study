#include "Grid.h"

#include <iostream>
#include <cmath>

int main()
{
    AOIGrid grid(10.0f);             // 每个网格 10x10
    Object  obj1(1, Vec2(15, 25));   // 10,20 --> 20 30
    Object  obj3(3, Vec2(100, 100)); // 90,90 --> 100 100
    Object  obj2(2, Vec2(22, 35));   // 20,30 --> 30 40
    Object  obj4(4, Vec2(35, 34));   // 30,30 --> 40 40
    Object  obj5(5, Vec2(42, 35));   // 40,30 --> 50 40
    Object  obj6(6, Vec2(25, 45));   // 20,40 --> 30 50
    Object  obj7(7, Vec2(35, 45));   // 30,40 --> 40 50
    Object  obj8(8, Vec2(45, 45));   // 40,40 --> 50 50
    Object  obj9(9, Vec2(25, 25));   // 20,20 --> 30 30
    Object  obj10(10, Vec2(35, 25)); // 30,20 --> 40 30
    Object  obj11(11, Vec2(45, 25)); // 40,20 --> 50 30
    Object  obj12(12, Vec2(45, 25)); // 40,20 --> 50 30
    Object  obj13(13, Vec2(55, 45)); // 50,40 --> 60 50
    Object  obj14(14, Vec2(5, 5));   // 0,0 --> 10 10

    // 检查目标对象是否在视锥内
    std::vector<Object> objects = {obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14};

    for (auto& obj : objects)
    {
        grid.AddObject(&obj);
    }

    auto c10 = grid.GetCell(obj10.position);
    std::cout << "对象 10 网格坐标：(" << c10.x << ", " << c10.y << ")" << std::endl;

    auto c1 = grid.GetCell(obj1.position);
    std::cout << "对象 1 网格坐标：(" << c1.x << ", " << c1.y << ")" << std::endl;

    std::cout << "对象 1 附近的对象：" << std::endl;
    for (auto obj : grid.GetNearbyObjects(&obj1, 1))
    {
        std::cout << " - Object ID: " << obj->id << std::endl;
    }

    std::cout << "移动对象 1 到 (30, 30)..." << std::endl;
    grid.MoveObject(&obj1, Vec2(30, 30));

    c1 = grid.GetCell(obj1.position);
    std::cout << "对象 1 网格坐标：(" << c1.x << ", " << c1.y << ")" << std::endl;
    std::cout << "对象 1 附近的对象（移动后）：" << std::endl;
    for (auto obj : grid.GetNearbyObjects(&obj1, 1))
    {
        std::cout << " - Object ID: " << obj->id << std::endl;
    }

    std::cout << "移动对象 1 到 (85, 95)..." << std::endl;
    grid.MoveObject(&obj1, Vec2(85, 95));

    c1 = grid.GetCell(obj1.position);
    std::cout << "对象 1 网格坐标：(" << c1.x << ", " << c1.y << ")" << std::endl;

    std::cout << "对象 1 附近的对象（移动后）：" << std::endl;
    for (auto obj : grid.GetNearbyObjects(&obj1, 1))
    {
        std::cout << " - Object ID: " << obj->id << std::endl;
    }

    // 设定摄像机视锥体 (x 轴范围: 0~30, y 轴范围: 0~30) 4X4-> (0,0) ~ (3,3)
    Frustum frustum(0, 30, 0, 30);

    // 获取视野内的网格
    std::vector<Vec2> visibleGrids = grid.GetVisibleGrids(frustum);
    std::cout << "可见网格数量: " << visibleGrids.size() << std::endl;
    for (const auto& cell : visibleGrids)
    {
        std::cout << "网格 (" << cell.x << ", " << cell.y << ") 在视野内" << std::endl;
    }

    // 获取视锥内的所有网格
    std::vector<Vec2> visibleCells = grid.GetAllVisibleCells(frustum);

    std::cout << "视锥范围内的网格数量: " << visibleCells.size() << std::endl;
    for (const auto& cell : visibleCells)
    {
        std::cout << "可见网格: (" << cell.x << ", " << cell.y << ")" << std::endl;
    }

    // 设置相机位置和方向
    Vec2  camPos(30, 30);     // 相机位置
    Vec2  direction(0, 1);    // 相机方向（朝上）
    float nearDist   = 10.0f; // 近裁剪距离
    float farDist    = 40.0f; // 远裁剪距离
    float fovDegrees = 60.0f; // FOV（视野角度）

    // 创建视锥
    Frustum2D frustum2D(camPos, direction, nearDist, farDist, fovDegrees);

    frustum2D.Print();

    grid.PrintVisibleCells(frustum2D);

    // 获取视野内的对象
    std::set<Object*> visibleObjects = grid.GetObjectsInFrustum(frustum2D);
    std::cout << "可见对象数量: " << visibleObjects.size() << std::endl;
    for (auto obj : visibleObjects)
    {
        std::cout << "对象 ID: " << obj->id << " 位置: (" << obj->position.x << ", " << obj->position.y << ")" << std::endl;
    }

    return 0;
}
