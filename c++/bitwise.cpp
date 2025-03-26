#include <iostream>
#include <cmath>

int main()
{

    for (int i = 0; i < 8; i++)
    {
        std::cout << "============================" << i << "============================" << std::endl;
        std::cout << (i & 1 << 0) << std::endl;
        std::cout << (i & 1 << 1) << std::endl;
        std::cout << (i & 1 << 2) << std::endl;
    }
}