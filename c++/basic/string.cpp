#include <iostream>
#include <cmath>
#include <string>

int main()
{

    // 0x7ffdd0454570 0x7ffdd0454590
    std::string str = "Hello World";

    std::string s = str;

    printf("%p %p\n", str.c_str(), s.c_str());

    const char* c = str.c_str();
    const char* d = c;
    const char* e = s.c_str();

    // 0x7ffdd0454570 0x7ffdd0454570 Hello World
    printf("%p %p %p %s\n", c, d, e, c);
}
