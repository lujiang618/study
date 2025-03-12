#include "Grid.h"

int main()
{
    Grid grid(10.0f); // 创建一个网格，每个 Cell 的大小为 10

    Object obj1 = {1, 15.0f, 20.0f, 25.0f}; // 物体1
    Object obj2 = {2, 16.0f, 21.0f, 25.5f}; // 物体2
    Object obj3 = {3, 50.0f, 50.0f, 50.0f}; // 物体3

    grid.insertObject(&obj1);
    grid.insertObject(&obj2);
    grid.insertObject(&obj3);

    // 查询某个 Cell 中的对象
    auto results = grid.queryObjects(15.0f, 20.0f, 25.0f);
    std::cout << "Objects in cell (15,20,25): ";
    for (auto obj : results)
    {
        std::cout << obj->id << " ";
    }
    std::cout << std::endl;

    // 查询邻近对象
    auto neighbors = grid.queryNeighbors(15.0f, 20.0f, 25.0f);
    std::cout << "Neighbor objects around (15,20,25): ";
    for (auto obj : neighbors)
    {
        std::cout << obj->id << " ";
    }
    std::cout << std::endl;

    return 0;
}