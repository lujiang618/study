#include <variant>
#include <string>
#include <iostream>

int main()
{
    std::variant<int, float, std::string> v;

    v = 42; // 存储 int

    v = "Hello, world!"; // 现在存储 std::string

    std::cout << std::get<std::string>(v) << std::endl;

    v = 3.14f; // 现在存储 float
    if (auto pval = std::get_if<int>(&v))
    {
        std::cout << "int value: " << *pval << std::endl;
    }
    else if (auto pval = std::get_if<float>(&v))
    {
        std::cout << "float value: " << *pval << std::endl;
    }
    else if (auto pval = std::get_if<std::string>(&v))
    {
        std::cout << "string value: " << *pval << std::endl;
    }

    v = "abc";
    try
    {
        float f = std::get<1>(v); // 获取第二个可能的类型（float）

        std::cout << "float value: " << f << std::endl;
    }
    catch (const std::bad_variant_access& e)
    {
        std::cerr << "Wrong index access: " << e.what() << std::endl;
    }
}