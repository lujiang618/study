#pragma once

#include <unordered_map>
#include <vector>
#include <set>
#include <cmath>
#include <iostream>
#include <algorithm>

#include "Frustum2D.h"

// 代表游戏中的对象
struct Object
{
    int  id;
    Vec2 position;

    Object(int id, Vec2 pos)
        : id(id)
        , position(pos)
    {
    }
};

class AOIGrid
{
public:
    AOIGrid(float gridSize);
    void AddObject(Object* obj);

    void RemoveObject(Object* obj);

    void MoveObject(Object* obj, Vec2 newPos);

    std::set<Object*> GetNearbyObjects(Object* obj, int range);

    std::vector<Vec2> GetVisibleGrids(const Frustum& frustum);

    std::vector<Vec2> GetAllVisibleCells(const Frustum& frustum);

    std::set<Object*> GetObjectsInFrustum(const Frustum2D& frustum);

    Vec2 CellToWorld(const Vec2& cell) const;

    std::vector<Vec2> GetVisibleCells(const Frustum2D& frustum);

    void PrintVisibleCells(const Frustum2D& frustum);

    Vec2 GetCell(Vec2 pos);

private:
    float                                          gridSize_;
    std::unordered_map<Vec2, std::vector<Object*>> grid_;
};
