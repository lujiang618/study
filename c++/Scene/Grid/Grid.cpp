#include "Grid.h"

#include <algorithm>
#include <iostream>
#include <string>

AOIGrid::AOIGrid(float gridSize)
    : gridSize_(gridSize)
{
}

// 计算对象所在的网格 Cell
Vec2 AOIGrid::GetCell(Vec2 pos)
{
    return Vec2(std::floor(pos.x / gridSize_), std::floor(pos.y / gridSize_));
}

// 添加对象到 AOI 网格
void AOIGrid::AddObject(Object* obj)
{
    Vec2 cell = GetCell(obj->position);
    grid_[cell].push_back(obj);
}

// 从 AOI 网格中移除对象
void AOIGrid::RemoveObject(Object* obj)
{
    Vec2  cell    = GetCell(obj->position);
    auto& objects = grid_[cell];
    objects.erase(std::remove(objects.begin(), objects.end(), obj), objects.end());
}

// 更新对象位置（移动）
void AOIGrid::MoveObject(Object* obj, Vec2 newPos)
{
    Vec2 oldCell = GetCell(obj->position);
    Vec2 newCell = GetCell(newPos);

    obj->position = newPos;

    // 只有跨 Cell 移动时才更新
    if (oldCell == newCell)
        return;

    RemoveObject(obj);
    AddObject(obj);
}

// 获取邻近范围内的对象
std::set<Object*> AOIGrid::GetNearbyObjects(Object* obj, int range)
{
    std::set<Object*> result;
    Vec2              center = GetCell(obj->position);

    std::cout << "center: " << center.x << ", " << center.y << std::endl;

    for (int dx = -range; dx <= range; ++dx)
    {
        for (int dy = -range; dy <= range; ++dy)
        {
            Vec2 cell(center.x + dx, center.y + dy);

            std::cout << "cell: " << cell.x << ", " << cell.y << std::endl;
            if (grid_.count(cell))
            {
                std::cout << "grid_[cell].size(): " << grid_[cell].size() << std::endl;
                for (auto obj : grid_[cell])
                {
                    std::cout << "obj: " << obj->id << " x:" << obj->position.x << " y:" << obj->position.y << std::endl;
                    result.insert(obj);
                }
                // result.insert(grid_[cell].begin(), grid_[cell].end());
            }
        }
    }

    return result;
}

std::vector<Vec2> AOIGrid::GetVisibleGrids(const Frustum& frustum)
{
    std::vector<Vec2> visibleGrids;

    for (const auto& entry : grid_)
    {
        Vec2  cell        = entry.first;
        float cellCenterX = (cell.x + 0.5f) * gridSize_;
        float cellCenterY = (cell.y + 0.5f) * gridSize_;

        if (frustum.Contains(cellCenterX, cellCenterY))
        {
            visibleGrids.push_back(cell);
        }
    }

    return visibleGrids;
}

std::vector<Vec2> AOIGrid::GetAllVisibleCells(const Frustum& frustum)
{
    std::vector<Vec2> visibleCells;

    // 计算视锥体的边界在网格索引中的范围
    int minX = static_cast<int>(std::floor(frustum.minX / gridSize_));
    int maxX = static_cast<int>(std::floor(frustum.maxX / gridSize_));
    int minY = static_cast<int>(std::floor(frustum.minY / gridSize_));
    int maxY = static_cast<int>(std::floor(frustum.maxY / gridSize_));

    // 遍历整个视锥范围内的网格
    for (int x = minX; x <= maxX; ++x)
    {
        for (int y = minY; y <= maxY; ++y)
        {
            visibleCells.emplace_back(x, y);
        }
    }

    return visibleCells;
}

Vec2 AOIGrid::CellToWorld(const Vec2& cell) const
{
    return Vec2(cell.x * gridSize_, cell.y * gridSize_);
}

std::vector<Vec2> AOIGrid::GetVisibleCells(const Frustum2D& frustum)
{
    std::vector<Vec2> visibleCells;

    for (auto& [cell, objects] : grid_)
    {
        Vec2 bottomLeft = CellToWorld(cell);
        Vec2 topRight   = bottomLeft + Vec2(gridSize_, gridSize_);

        AABB aabb{bottomLeft, topRight};

        // 如果所有角都不在视锥内，跳过
        if (frustum.Intersect1(aabb))
        {
            visibleCells.push_back(cell);
        }
    }

    return visibleCells;
}

std::set<Object*> AOIGrid::GetObjectsInFrustum(const Frustum2D& frustum)
{
    std::set<Object*> result;

    for (auto& [cell, objects] : grid_)
    {
        Vec2 bottomLeft = CellToWorld(cell);
        Vec2 topRight   = bottomLeft + Vec2(gridSize_, gridSize_);

        AABB aabb{bottomLeft, topRight};

        if (frustum.Intersect1(aabb))
        {
            result.insert(objects.begin(), objects.end());
        }
    }

    return result;
}

void AOIGrid::PrintVisibleCells(const Frustum2D& frustum)
{
    auto cells = GetVisibleCells(frustum);

    std::cout << "frustum2D可见网格数量: " << cells.size() << std::endl;
    for (auto& cell : cells)
    {
        std::cout << "cell:" << cell << std::endl;
    }
}