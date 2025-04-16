#include <iostream>
#include <array>
#include <string>

int main()
{
    std::array<std::string, 3> arr = {"Hello", "World", "!"};

    for (auto& str : arr)
    {
        std::cout << str << " "<< std::endl;

        str = "a new string";
    }

    std::cout << "============================================" << std::endl;

    for (auto& str : arr)
    {
        std::cout << str << " " << std::endl;
    }

    return 0;
}
