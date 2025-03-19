#include <iostream>
#include <string>
#include <unordered_map>

uint8_t* b;

struct abc {
    int a;
    int b;
    int c;
};

int main() {
    std::unordered_map<int, abc> m;

    m[1].a = 1;
    m[3].c = 1;
    m[4].c = 1;

    std::cout << m[1].a << std::endl;
    std::cout << m[3].a << std::endl;
    std::cout << m[4].a << std::endl;
}
