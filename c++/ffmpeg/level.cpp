#include <iostream>
#include <cmath>
#include <string>

float CalculateH264Level(int width, int height, int fps)
{
    uint64_t pixelsPerSecond = static_cast<uint64_t>(width) * height * fps;

    if (pixelsPerSecond <= 66355200) return 4.1;

    if (pixelsPerSecond <= 27648000) return 4.2;

    if (pixelsPerSecond <= 251658240) return 5.1;

    if (pixelsPerSecond <= 503316480) return 5.2;

    return 5.2;
}

int main()
{
    int width = 2560;   // 输入分辨率宽度
    int height = 1440;  // 输入分辨率高度
    int fps = 30;       // 输入帧率

    auto level = CalculateH264Level(width, height, fps);
    std::cout << "Recommended H.264 Level: " << std::to_string(level) << std::endl;

    return 0;
}
