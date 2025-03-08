#include <iostream>
#include <functional>

int main() {
    // 对 int 类型哈希
    std::hash<int> intHasher;
    size_t intHash = intHasher(42);
    std::cout << "Hash of 42: " << intHash << std::endl;

    // 对 double 类型哈希
    std::hash<double> doubleHasher;
    size_t doubleHash = doubleHasher(3.14);
    std::cout << "Hash of 3.14: " << doubleHash << std::endl;

    std::hash<std::string> stringHasher;
    size_t stringHash = stringHasher("Hello, World!");
    std::cout << "Hash of \"Hello, World!\": " << stringHash << std::endl;

    return 0;
}