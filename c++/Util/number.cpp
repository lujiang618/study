#include <iostream>
#include <cmath>
#include <string>
#include <cstdint>

int64_t CombineFloatToInt64(float higher, float lower)
{
    int64_t key = static_cast<int64_t>(higher) << 32;

    // 0xFFFFFFFF 确保 low 只占用低 32 位
    key |= static_cast<int64_t>(lower) & 0xFFFFFFFF;

    return key;
};

int main()
{

    auto a = CombineFloatToInt64(0.0f, 0.0f);
    auto b = CombineFloatToInt64(1.0f, 0.0f);
    auto c = CombineFloatToInt64(2.0f, 0.0f);
    auto d = CombineFloatToInt64(2.0f, 1.0f);
    auto e = CombineFloatToInt64(2.0f, 2.0f);

    printf("%ld %ld %ld %ld %ld\n", a, b, c, d, e);


    return 0;
}
