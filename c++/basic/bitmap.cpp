#include <stdint.h>
#include <iostream>

uint16_t pack_data(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    return (a << 12) | (b << 8) | (c << 4) | d;
}

// 从打包的数据中解包出第3个值（从0开始计数，这里是c）
uint8_t unpack_data_c(uint16_t packed)
{
    return (packed >> 4) & 0x0F; // 右移后使用掩码0x0F（二进制1111）取出低4位
}

int main()
{
    uint8_t a = 1, b = 2, c = 3, d = 4;
    uint16_t packed = pack_data(a, b, c, d);
    std::cout << "Packed data: " << std::hex << packed << std::dec << std::endl;

    uint8_t unpacked_c = unpack_data_c(packed);
    std::cout << "Unpacked c: " << (int)unpacked_c << std::endl;

    return 0;
}