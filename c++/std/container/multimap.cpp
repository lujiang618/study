#include <iostream>
#include <map>
#include <functional>

using Callback = std::function<void()>;

int main()
{
    std::multimap<int, Callback> callbackMap;

    // 添加回调（优先级越高，数字越小）
    callbackMap.emplace(2, []
    { std::cout << "Priority 2\n"; });
    callbackMap.emplace(1, []
    { std::cout << "Priority 1\n"; }); // 高优先级
    callbackMap.emplace(3, []
    { std::cout << "Priority 3\n"; }); // 低优先级

    // 按优先级顺序执行
    for (const auto& [priority, cb] : callbackMap)
    {
        cb(); // 输出顺序: 1 → 2 → 3
    }

    return 0;
}