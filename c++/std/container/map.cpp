#include <iostream>
#include <map>

void test1()
{
    std::map<int, int> m = {{1, 2}, {3, 4}};

    // 这里at不会崩，因为m[4] 给了默认值0
    std::cout << m[4] << std::endl;
    std::cout << m.at(4) << std::endl;

    // 这里at会崩， 因为2不存在
    std::cout << m.at(2) << std::endl;
    std::cout << m[2] << std::endl;
}

void test2()
{
    struct A
    {
        int a;
    };

    std::map<A*, bool> m;

    A* a1 = new A();
    A* a2 = new A();
    A* a3 = new A();

    m[a1] = true;
    m[a2] = false;

    std::cout << m[a2] << std::endl;

    if (m.find(a1) != m.end())
    {
        std::cout << "find a1" << std::endl;
    }

    if (m.find(a3) == m.end())
    {
        std::cout << "not find a3" << std::endl;
    }
}

int main()
{
    test2();
}