#include <iostream>
#include <string>

uint8_t* b;

void test() {
    uint8_t* buffer = new uint8_t[20];;

    for (int i = 0; i < 10; i++) {
        buffer[i] = i;
    }

    std::cout<<sizeof(buffer)<<std::endl;

    b = buffer;

    // delete[] buffer;
}

int main() {
    test();

    // for(int i = 0; i < 10; i++ ) {
    //     std::cout << std::to_string(b[i]) << std::endl;
    // }
}
