#include <variant>
#include <string>
#include <iostream>
#include <sstream>

int64_t combineInt32ToInt64(int32_t high, int32_t low)
{
    // 将 high 左移 32 位，放到 int64 的高位
    int64_t result = static_cast<int64_t>(high) << 32;
    // 将 low 放到 int64 的低位
    result |= static_cast<int64_t>(low) & 0xFFFFFFFF; // 确保 low 只占用低 32 位
    return result;
}

int32_t extractHighInt32FromInt64(int64_t combined)
{
    // 提取高位：将 combined 右移 32 位
    return static_cast<int32_t>(combined >> 32);
}

int32_t extractLowInt32FromInt64(int64_t combined)
{
    // 提取低位：将 combined 与 0xFFFFFFFF 进行按位与操作
    return static_cast<int32_t>(combined & 0xFFFFFFFF);
}

int main()
{
    auto a = combineInt32ToInt64(1,2);

    auto b = extractHighInt32FromInt64(a);
    auto c = extractLowInt32FromInt64(a);

    printf("%ld %d %d\n", a, b, c);

    return 0;
}
