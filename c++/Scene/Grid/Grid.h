#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <algorithm>

// 网格单元（Cell）的键类型
struct GridKey
{
    int x, y, z;

    bool operator==(const GridKey& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }
};

// 自定义哈希函数（用于存储 GridKey）
struct GridKeyHash
{
    std::size_t operator()(const GridKey& key) const
    {
        return std::hash<int>()(key.x) ^ std::hash<int>()(key.y) ^ std::hash<int>()(key.z);
    }
};

// 3D 物体结构
struct Object
{
    int   id;      // 物体 ID
    float x, y, z; // 物体位置
};

// 网格（Grid）类
class Grid
{
private:
    float                                                          cellSize; // 网格单元大小
    std::unordered_map<GridKey, std::vector<Object*>, GridKeyHash> cells;    // 存储对象

    // 计算对象所在的 GridKey
    GridKey getGridKey(float x, float y, float z) const
    {
        return {static_cast<int>(std::floor(x / cellSize)),
                static_cast<int>(std::floor(y / cellSize)),
                static_cast<int>(std::floor(z / cellSize))};
    }

public:
    Grid(float size)
        : cellSize(size)
    {
    }

    // 插入对象
    void insertObject(Object* obj)
    {
        GridKey key = getGridKey(obj->x, obj->y, obj->z);
        cells[key].push_back(obj);
    }

    // 查询对象所在的 Cell
    std::vector<Object*> queryObjects(float x, float y, float z)
    {
        GridKey key = getGridKey(x, y, z);
        if (cells.find(key) != cells.end())
        {
            return cells[key];
        }
        return {};
    }

    // 查询邻近 Cell 中的对象（8 邻域搜索）
    std::vector<Object*> queryNeighbors(float x, float y, float z)
    {
        std::vector<Object*> result;
        GridKey              key = getGridKey(x, y, z);

        for (int dx = -1; dx <= 1; dx++)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                for (int dz = -1; dz <= 1; dz++)
                {
                    GridKey neighborKey = {key.x + dx, key.y + dy, key.z + dz};
                    if (cells.find(neighborKey) != cells.end())
                    {
                        result.insert(result.end(), cells[neighborKey].begin(), cells[neighborKey].end());
                    }
                }
            }
        }
        return result;
    }

    // 移除对象
    void removeObject(Object* obj)
    {
        GridKey key  = getGridKey(obj->x, obj->y, obj->z);

        auto&   cell = cells[key];

        cell.erase(std::remove(cell.begin(), cell.end(), obj), cell.end());
    }
};