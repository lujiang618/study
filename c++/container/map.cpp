#include <iostream>
#include <map>

int main()
{

    std::map<int, int> m = {{1, 2}, {3, 4}};

    // 这里at不会崩，因为m[4] 给了默认值0
    std::cout<<m[4]<<std::endl;
    std::cout << m.at(4) << std::endl;

    // 这里at会崩， 因为2不存在
    std::cout << m.at(2) << std::endl;
    std::cout<<m[2]<<std::endl;
}