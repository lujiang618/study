#include <functional>
#include <iostream>

void callback5(void* a, double b)
{
    std::cout << "Callback 5, received: " << *static_cast<int*>(a) << " and " << b << std::endl;
}

int main()
{
    int   a = 20;
    void* b = &a;

    callback5(b, 3.14);

    // 修改 std::function 的签名为 void(void*, double)
    auto c5 = std::bind(callback5, b, std::placeholders::_1);

    // 调用函数
    c5(3.14);

    return 0;
}
