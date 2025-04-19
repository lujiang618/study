#include <vector>
#include <iostream>

#include <cmath>

int main()
{

    float a = 0.0f;
    float b = 0.0f;

    float c = 1.0f;

    std::cout << c / a << std::endl; // inf
    std::cout << b / a << std::endl; // -nan

    std::cout << std::isnan(c / a) << std::endl;
    std::cout << std::isnan(b / a) << std::endl;

    float d = b / a;

    std::cout << b * d << std::endl;
    std::cout << c * d << std::endl;

    return 0;
}